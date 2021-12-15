#include "pipe_networking.h"

static void sighandler(int sig) {

    if (sig == SIGINT) {
        printf("Exiting server...\n");
        exit(0);
    }

}

int main() {
    
    signal(SIGINT, sighandler);

    int to_client;
    int from_client;
    
    from_client = server_handshake( &to_client );

    char *buffer = calloc(BUFFER_SIZE, sizeof(char));

    while (1) {

        int fd = read(from_client, buffer, BUFFER_SIZE);
        
        if (fd) {
            int i;
            for (i = 0; i < strlen(buffer); i++) {
                if (isalpha(buffer[i])) {
                    buffer[i] = toupper(buffer[i]);
                }
            }

            write(to_client, buffer, strlen(buffer));
        }
        else {
            main();
        }

    }


    return 0;
}