#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>

#define MESSAGE_BUFFER_SIZE 100

struct AcceptedClient {
  struct sockaddr client_address;
  int client_socket_fd; // or the error no
  bool isAccepted;
};

struct AcceptedClient* accept_incoming_connection(const int server_socket_fd) {
  struct sockaddr client_address;
  socklen_t client_address_size = sizeof(struct sockaddr);
  int client_socket_fd = accept(server_socket_fd, &client_address, &client_address_size);

  struct AcceptedClient* accepted_client = malloc(sizeof (struct AcceptedClient));
  accepted_client->client_address = client_address;
  accepted_client->client_socket_fd = client_socket_fd;
  accepted_client->isAccepted = client_socket_fd > 0;

  return accepted_client;
}

void respond_to_client(const struct AcceptedClient* accepted_client) {
  char buffer[MESSAGE_BUFFER_SIZE];
  while (true) {
    ssize_t numberOfBytesReceived = recv(accepted_client->client_socket_fd, buffer, MESSAGE_BUFFER_SIZE, 0);
    printf("Received: %ld\n", numberOfBytesReceived);
    if (numberOfBytesReceived == -1) {
        printf("Error occurred while receiving data from client");
    } else {
      // buffer[numberOfBytesReceived] = 0;
      printf("Message: %s", buffer);
    }
  }
}

pthread_t create_thread_for_client(struct AcceptedClient* accepted_client) {
  pthread_t thread;
  pthread_create(&thread, NULL, respond_to_client, accepted_client);
  printf("Created a thread for client\n");

  return thread;
}

static void catch_function(int signal_no) {
  switch (signal_no) {
    case SIGINT:
      printf("Shutting down server.");
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

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd < 0) {
    printf("Couldn't open the socket\n");
    exit(1);
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(2000);
  inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);

  int result = bind(socket_fd, &address, sizeof address);

  if (result != 0) {
    printf("Couldn't bind to the socket. Closing the socket...\n");
    int closed_status = close(socket_fd);
    if (closed_status == 0) {
      printf("Closed the socket\n");
    } else {
      printf("Couldn't close the socket\n");
    }
    exit(1);
  }

  int listening_port = ntohs(address.sin_port);
  int listen_status = listen(socket_fd, 10);
  if (listen_status != 0) {
    printf("Couldn't listen on port %d. Closing the socket...\n", listening_port);
    int closed_status = close(socket_fd);
    if (closed_status == 0) {
      printf("Closed the socket\n");
    } else {
      printf("Couldn't close the socket\n");
    }
    exit(1);
  }

  printf("Started the listening on http://localhost:%d\n", listening_port);

  while (true) {
    struct AcceptedClient* accepted_client = accept_incoming_connection(socket_fd);
    if (!accepted_client->isAccepted) {
      printf("Couldn't establish connection with client.\n");
      continue;
    }
   create_thread_for_client(accepted_client);
  }
  shutdown(socket_fd, SHUT_RDWR);
  return 0;
}
