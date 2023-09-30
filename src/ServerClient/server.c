#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h> //socket steps, and protocols
#include <arpa/inet.h>
#include <sys/ioctl.h>

#define BUFF_SIZE 4096

void handle_connection(int client_sock) {
  //printf("We've received a new connection!\n");
  //close(client_sock);
  //return;
  //Buffers to read and write input and output
  char input[BUFF_SIZE];
  bzero(input, BUFF_SIZE);
  char username[BUFF_SIZE];
  bzero(username, BUFF_SIZE);
  int n;

  printf("We've received a new connection!\n");

  read(client_sock, username, BUFF_SIZE);
  username[strcspn(username, "\n")] = 0;

  while (1) {
    read(client_sock, input, BUFF_SIZE);
    if (strncmp("\n", input, 1) == 0) {
      printf("Server Exiting...\n");
      break;
    } 
    printf("%s: %s", username, input);
    bzero(input, BUFF_SIZE);
    n = 0; 
  }

  close(client_sock);
  printf("The connection has been closed!\n");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Not enough arguments.\n");
    printf("Please include a port number.\n");
    return -1;
  }
  
  unsigned short PORT = (unsigned short) atoi(argv[1]);

  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0) {
    printf("Socket creation failed!\n");
    return -1;
  }
  int opt_value = 1;
  int status = setsockopt(server_sock, SOL_SOCKET, SO_REUSEPORT, &opt_value, sizeof(opt_value));
  if (status < 0) {
    printf("Unable to set options correctly.");
    return -1;
  }

  struct sockaddr_in server_addr;
  //& refers to the pointer of this variable
  //This function clears out the data at that spot.
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  //Changes the address to be the variable type for this struct
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if(bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    printf("Unable to bind\n");
    return -1;
  }
  printf("Server Socket has been bound!\n");
  //The 5 there is the number of connections you can have
  //ongoing at any one time.
  if(listen(server_sock, 5) < 0) {
    printf("Unable to bind\n");
    return -1;
  }
  printf("Server Socket is listening on port %hu!\n", PORT);

  int addr_size = sizeof(struct sockaddr_in);
  int client_sock;
  struct sockaddr_in client_addr;

  while (1) {
    printf("Waiting for connections...\n");

    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);
    if (client_sock < 0) {
      printf("Connected!\n");
      return -1;
    }
    handle_connection(client_sock);
  }

  close(server_sock);

  return 0; 
}

