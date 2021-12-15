#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  mkfifo(WKP, 0644);

  int from_client = open(WKP, O_RDONLY);
  char *secret_name = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
  read(from_client, secret_name, HANDSHAKE_BUFFER_SIZE);
  printf("Server read secret name: %s\n", secret_name);
  remove(WKP);

  *to_client = open(secret_name, O_WRONLY);
  write(*to_client, ACK, strlen(ACK));

  char *response = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
  read(from_client, response, HANDSHAKE_BUFFER_SIZE);
  printf("Receieved response: %s\n", response);

  return from_client;

}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

    char *secret_name = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
    sprintf(secret_name, "%d", getpid());
    mkfifo(secret_name, 0644);

    *to_server = open(WKP, O_WRONLY);
    write(*to_server, secret_name, strlen(secret_name));

    int from_server = open(secret_name, O_RDONLY);
    char *ack = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
    read(from_server, ack, HANDSHAKE_BUFFER_SIZE);

    printf("Received acknowledgment: %s\n", ack);
    remove(secret_name);
    if (!strcmp(ack, ACK)) {
        write(*to_server, "Success", strlen("Success"));
    }

    return from_server;
}