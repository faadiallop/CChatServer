#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h> //socket steps, and protocols
#include <arpa/inet.h>

#define BUFF_SIZE 4096

void send_message(int client_sock, char input[]) {

  int i = 0;
  //You can only do this comparison because \n is in single quotes.
  //The single quotes indicate a single character rather than a
  //char *.
  while ((input[i++] = getchar()) != '\n');
  printf("%s", input);

  size_t curr_bytes_written = 0;
  size_t num_to_write = strlen(input);
  while (curr_bytes_written < num_to_write) {
    size_t num_written = write(client_sock, input + curr_bytes_written, num_to_write - curr_bytes_written);
    if (num_written == -1) {
      printf("Unable to write.\n");
      return;
    }
    curr_bytes_written += num_written;
  }

}

int main(int argc, char *argv[]) {
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
  int status;

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
  status = connect(client_sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
  if (status < 0) {
    printf("%s\n", strerror(errno));
    printf("Unable to connect to server\n");
    return -1;
  }

  bool message_prompt = false;
  while (1) {
    if (message_prompt == false) {
      printf("What is your username? ");
    } else {
      printf("What is your message: ");
    }
    message_prompt = true;
    send_message(client_sock, input);
    if (strncmp("\n", input, 1) == 0) {
      printf("You have exited the group chat!\n");
      break;
    }
    bzero(input, BUFF_SIZE);
  }
  return 0;
}

