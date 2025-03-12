#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>

#include "./fs-utils.c"

#define MESSAGE_BUFFER_SIZE 5000

struct AcceptedClient {
  struct sockaddr client_address;
  int client_socket_fd; // or the error no
  bool isAccepted;
};

int server_socket_fd;

struct AcceptedClient* accept_incoming_connection() {
  struct sockaddr client_address;
  socklen_t client_address_size = sizeof(struct sockaddr);
  int client_socket_fd = accept(server_socket_fd, &client_address, &client_address_size);

  struct AcceptedClient* accepted_client = malloc(sizeof (struct AcceptedClient));
  accepted_client->client_address = client_address;
  accepted_client->client_socket_fd = client_socket_fd;
  accepted_client->isAccepted = client_socket_fd > 0;

  return accepted_client;
}

void* respond_to_client(const struct AcceptedClient* accepted_client) {
  char buffer[MESSAGE_BUFFER_SIZE];
  char response[MESSAGE_BUFFER_SIZE];

  while (true) {
    ssize_t numberOfBytesReceived = read(accepted_client->client_socket_fd, buffer, MESSAGE_BUFFER_SIZE);
    if (numberOfBytesReceived == MESSAGE_BUFFER_SIZE) {
      sprintf(buffer,
        "HTTP/1.1 413 Request Entity Too Large\r\n"
        "Content-Type: text/plaintext\r\n"
        "\r\n"
        "Maximum request size accepted by Cerve is %d bytes.", MESSAGE_BUFFER_SIZE);
      send(accepted_client->client_socket_fd, buffer, strlen(buffer), 0);
      close(accepted_client->client_socket_fd);
      free(accepted_client);
      return NULL;
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
    char *file_path = convert_url_segment_to_file_location(url_segment);

    printf(":: %s %s\n", method, url_segment);

    char response_message[512];
    sprintf(response_message, "You requested %s %s.\r\n%s is the target file.\r\n", method, url_segment, file_path);
    int response_length = strlen(response_message);
    sprintf(response,
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-length: %d\r\n"
      "\r\n"
      "%s", response_length, response_message);
    free(file_path);

    int sent_bytes = send(accepted_client->client_socket_fd, response, strlen(response), 0);

    if (sent_bytes == -1) {
      perror("Error sending message to client");
    }
  }
  return NULL;
}

void create_thread_for_client(struct AcceptedClient* accepted_client) {
  if (!accepted_client->isAccepted) {
    return;
  }
  pthread_t thread;
  pthread_create(&thread, NULL, respond_to_client, accepted_client);
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

int subcmd_serve() {
  if (signal(SIGINT, catch_function) == SIG_ERR) {
    fputs("Error occurred while setting a signal handler.\n",stderr);
    return EXIT_FAILURE;
  }

  server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket_fd < 0) {
    printf("Couldn't open the socket\n");
    exit(1);
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(2000);
  inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);

  const int opt = 1;
  setsockopt(server_socket_fd, SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));

  int result = bind(server_socket_fd, &address, sizeof address);

  if (result != 0) {
    perror("Couldn't bind to the socket");
    int closed_status = close(server_socket_fd);
    if (closed_status == 0) {
      printf("Closed the socket\n");
    } else {
      printf("Couldn't close the socket\n");
    }
    exit(1);
  }

  int listening_port = ntohs(address.sin_port);
  int listen_status = listen(server_socket_fd, 10);
  if (listen_status != 0) {
    printf("Couldn't listen on port %d. Closing the socket...\n", listening_port);
    int closed_status = close(server_socket_fd);
    if (closed_status == 0) {
      printf("Closed the socket\n");
    } else {
      printf("Couldn't close the socket\n");
    }
    exit(1);
  }

  printf("Started the listening on http://localhost:%d\n", listening_port);

  while (true) {
    struct AcceptedClient* accepted_client = accept_incoming_connection();
    if (!accepted_client->isAccepted) {
      printf("Couldn't establish connection with client.\n");
      continue;
    }
   create_thread_for_client(accepted_client);
  }
}
