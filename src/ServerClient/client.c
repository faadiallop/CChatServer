#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h> //socket steps, and protocols
#include <arpa/inet.h>

#define BUFF_SIZE 4096


int main(int argc, char *argv[])
{
  if (argc != 3) {
    printf("Not enough arguments.\n");
    return -1;
  }
  
  //SANITIZE THESE INPUTS!!!!!
  char* IP_ADDR = argv[1];
  if (strcmp(argv[1], "loopback") == 0) {
    //You're allowed to redefine this variable because
    //it generates another string literal corresponding
    //to "127.0.0.1" which IP_ADDR is then pointed to.
    IP_ADDR = "127.0.0.1";
  }
  unsigned short PORT = (unsigned short) atoi(argv[2]);
  //Buffers to read and write input and output
  char output[BUFF_SIZE+1];
  //YOU MIGHT NOT NEED THE OUTPUT BUFFER!!!!!
  char input[BUFF_SIZE+1];

  bzero(&output, sizeof(output));
  int client_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (client_sock < 0) {
    printf("Socket creation error\n");
    return -1;
  }

  struct sockaddr_in server_addr;
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
  server_addr.sin_port = htons(PORT);

  if (connect(client_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    printf("%s\n", strerror(errno));
    printf("Unable to connect to server\n");
    return -1;
  }

  //Return statement to prematurely stop program.
  return 0;

  sprintf(output, "Hello World!\n");
  size_t output_bytes = sizeof(output);

  if (write(client_sock, output, output_bytes) != output_bytes) {
    printf("Unable to write.\n");
    return -1;
  }

  
  return 0;
}

