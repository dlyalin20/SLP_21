#include "pipe_networking.h"

static void sighandler(int sig) {

    if (sig == SIGINT) {
        printf("Exiting client...\n");
        exit(0);
    }

}

int main() {

    signal(SIGINT, sighandler);

    int to_server;
    int from_server;
    
    from_server = client_handshake( &to_server );

    while (1) {

        char *buffer = calloc(BUFFER_SIZE, sizeof(char));
        
        printf("Please input, maxlength %d: ", BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        
        if (!strcmp(buffer, "exit")) {
            sighandler(SIGINT);
        }

        write(to_server, buffer, strlen(buffer));

        read(from_server, buffer, strlen(buffer));

        printf("ROT13: %s\n", buffer);

    }

    return 0;
}