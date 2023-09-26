#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> //socket steps, and protocols
#include <arpa/inet.h>

#define BUFF_SIZE 4096

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Not enough arguments.\n");
    return -1;
  }
  
  //SANITIZE THESE INPUTS
  char* IP_ADDR = argv[1];
  if (strcmp(argv[1], "loopback") == 0) {
    //You're allowed to redefine this variable because
    //it generates another string literal corresponding
    //to "127.0.0.1" which IP_ADDR is then pointed to.
    IP_ADDR = "127.0.0.1";
  }
  //Buffers to read and write input and output
  char output[BUFF_SIZE+1];
  char input[BUFF_SIZE+1];
  unsigned short PORT = (unsigned short) atoi(argv[2]);
  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0) {
    printf("Unable to bind\n");
    return -1;
  }

  struct sockaddr_in server_addr;
  //& refers to the pointer of this variable
  //This function clears out the data at that spot.
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  //Changes the address to be the variable type for this struct
  server_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
  server_addr.sin_port = htonl(PORT);

  if(bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    printf("Unable to bind\n");
    return -1;
  }
  printf("Server Socket has been bound!\n")
  //The 5 there is the number of connections you can have
  //ongoing at any one time.
  if(listen(server_sock, 5) < 0) {
    printf("Unable to bind\n");
    return -1;
  }
  printf("Server Socket is listening here: %s:%s!\n", IP_ADDR, PORT)






  return 0; 
}
