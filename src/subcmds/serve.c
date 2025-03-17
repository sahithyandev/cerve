#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cwalk.h>

#include "../utils/fs.c"
#include "../utils/http.c"

#define REQUEST_BUFFER_SIZE 5000
#define RESPONSE_BUFFER_SIZE 1000

extern int LOG_LEVEL;
extern char SERVING_DIR[PATH_MAX];

struct AcceptedClient {
	struct sockaddr client_address;
	int client_socket_fd; // or the error no
	bool isAccepted;
};

int server_socket_fd;

struct AcceptedClient *accept_incoming_connection() {
	struct sockaddr client_address;
	socklen_t client_address_size = sizeof(struct sockaddr);
	int client_socket_fd =
		accept(server_socket_fd, &client_address, &client_address_size);

	struct AcceptedClient *accepted_client =
		malloc(sizeof(struct AcceptedClient));
	accepted_client->client_address = client_address;
	accepted_client->client_socket_fd = client_socket_fd;
	accepted_client->isAccepted = client_socket_fd > 0;

	return accepted_client;
}

void send_file_to_client(int client_socket_fd, const char *file_path) {
	FILE *opened_file = fopen(file_path, "r");
	char response[RESPONSE_BUFFER_SIZE];

	char *extension;
	size_t length;
	cwk_path_get_extension(file_path, &extension, &length);

	create_file_response_headers(response, extension + 1,
								 file_size(opened_file));
	int sent_bytes = send(client_socket_fd, response, strlen(response), 0);
	if (sent_bytes == -1) {
		perror("Error sending message to client");
	}

	if (opened_file == NULL) {
		perror("Error opening file");
	}
	while (fgets(response, RESPONSE_BUFFER_SIZE, opened_file) != NULL) {
		sent_bytes = send(client_socket_fd, response, strlen(response), 0);
		if (sent_bytes == -1) {
			perror("Error sending message to client");
		}
	}
	fclose(opened_file);
}

void respond_to_client(struct AcceptedClient *accepted_client) {
	char buffer[REQUEST_BUFFER_SIZE];

	while (true) {
		ssize_t numberOfBytesReceived = read(accepted_client->client_socket_fd,
											 buffer, REQUEST_BUFFER_SIZE);
		if (numberOfBytesReceived == REQUEST_BUFFER_SIZE) {
			sprintf(buffer,
					"HTTP/1.1 %hi %s\r\n"
					"Content-Type: text/plaintext\r\n"
					"\r\n"
					"Maximum request size accepted by Cerve is %d bytes.",
					413, status_code_to_str(413), REQUEST_BUFFER_SIZE);
			send(accepted_client->client_socket_fd, buffer, strlen(buffer), 0);
			close(accepted_client->client_socket_fd);
			free(accepted_client);
			return;
		}

		if (numberOfBytesReceived == -1) {
			printf("Error occurred while receiving data from client\n");
			continue;
		}
		if (numberOfBytesReceived == 0) {
			break;
		}
		buffer[numberOfBytesReceived] = 0;
		char *method = strtok(buffer, " ");
		char *url_segment = strtok(NULL, " ");
		url_segment = normalize_url_segment(url_segment);

		char file_path[PATH_MAX];
		cwk_path_join(SERVING_DIR, url_segment, file_path, sizeof(file_path));

		signed short int status_code = 200;

		if (access(file_path, F_OK) != 0) {
			char response[RESPONSE_BUFFER_SIZE];

			// file doesn't exist
			status_code = 404;
			create_response(response, status_code, NULL);

			printf("%hi:: %s %s\n", status_code, method, url_segment);
			int sent_bytes = send(accepted_client->client_socket_fd, response,
								  strlen(response), 0);
			if (sent_bytes == -1) {
				perror("Error sending message to client");
			}
			continue;
		}

		send_file_to_client(accepted_client->client_socket_fd, file_path);
		printf("%hi:: %s %s\n", status_code, method, url_segment);
	}
}

void create_thread_for_client(struct AcceptedClient *accepted_client) {
	if (!accepted_client->isAccepted) {
		return;
	}
	pthread_t thread;
	pthread_create(&thread, NULL, respond_to_client, accepted_client);
}

void close_socket() {
	const int closed_status = close(server_socket_fd);
	if (closed_status == 0) {
		printf("Closed the socket\n");
	} else {
		perror("Couldn't close the socket\n");
	}
}

static void catch_function(int signal_no) {
	switch (signal_no) {
	case SIGINT:
		printf("\nGracefully shutting down server...\n");
		shutdown(server_socket_fd, SHUT_RDWR);
		close(server_socket_fd);
		break;
	default:
		printf("Exiting with error code %d\n", signal_no);
	}
	exit(signal_no);
}

int subcmd_serve(const int PORT) {
	if (signal(SIGINT, catch_function) == SIG_ERR) {
		fputs("Error occurred while setting a signal handler.\n", stderr);
		return EXIT_FAILURE;
	}

	server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket_fd < 0) {
		printf("Couldn't open the socket\n");
		exit(1);
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);

	const int opt = 1;
	setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	const int result = bind(server_socket_fd, &address, sizeof address);

	if (result != 0) {
		perror("Couldn't bind to the socket");
		close_socket();
		exit(1);
	}

	const int listening_port = ntohs(address.sin_port);
	const int listen_status = listen(server_socket_fd, 10);
	if (listen_status != 0) {
		fprintf(stderr, "Couldn't listen on port %d. Closing the socket...\n",
				listening_port);
		close_socket();
		exit(1);
	}

	printf("HTTP Server started on: http://localhost:%d\n", listening_port);
	printf("Serving: %s\n", SERVING_DIR);

	while (true) {
		struct AcceptedClient *accepted_client = accept_incoming_connection();
		if (!accepted_client->isAccepted) {
			fputs("Couldn't establish connection with client.\n", stderr);
			continue;
		}
		create_thread_for_client(accepted_client);
	}
}
