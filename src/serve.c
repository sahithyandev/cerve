#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define MESSAGE_BUFFER_SIZE 100

int start_server() {

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
    printf("Couldn't connect to the socket. Closing the socket... (%d)\n", result);
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

  struct sockaddr_in client_address;
  int client_address_size = sizeof(struct sockaddr_in);
  int client_socket_fd = accept(socket_fd, &client_address, &client_address_size);

  if (client_socket_fd < 0) {
    printf("Couldn't establish connection with client.\n");
    exit(1);
  }

  char buffer[MESSAGE_BUFFER_SIZE];
  while (true) {
    ssize_t numberOfBytesReceived = recv(client_socket_fd, buffer, MESSAGE_BUFFER_SIZE, 0);
    if (numberOfBytesReceived == -1) {
        printf("Error occurred while receiving data from client");
    } else {
      buffer[numberOfBytesReceived] = 0;
      printf("Message == %s\n", buffer);
    }
  }

  close(client_socket_fd);
  shutdown(socket_fd, SHUT_RDWR);

  return 0;
}
