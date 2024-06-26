#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define FIFO_WRITE "fifo_write"
#define FIFO_READ "fifo_read"

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int usuario = 0;
    int option;
    while ((option = getopt(argc, argv, "wr")) != -1){
        switch (option)
        {
        case 'w':
            usuario = 0;
            break;
        
        case 'r':
            usuario = 1;
            break;
        
        default:
            usuario = 0;
            break;
        }
    }

    int fd_write = -1, fd_read = -1;
    char buffer[256];
    if (mkfifo(FIFO_WRITE, 0666) == -1 && errno != EEXIST){
        handle_error("FIFO_WRITE");
    }
    if (mkfifo(FIFO_READ, 0666) == -1 && errno != EEXIST){
        handle_error("FIFO_READ");
    }

    if (usuario == 0){
        if ((fd_write = open(FIFO_WRITE, O_WRONLY))==-1){
            handle_error("fd_write");
        }

        if ((fd_read = open(FIFO_READ, O_RDONLY))==-1){
            handle_error("fd_read");
        }
    }
    else{
        if ((fd_write = open(FIFO_WRITE, O_RDONLY))==-1){
            handle_error("fd_write");
        }

        if ((fd_read = open(FIFO_READ, O_WRONLY))==-1){
            handle_error("fd_read");
        }
    }

    while (1) {
        fd_set rfds;
        struct timeval tv;
        int retval;

        /* Watch stdin (fd 0) to see when it has input. */

        FD_ZERO(&rfds);
        FD_SET(fd_read, &rfds);
        FD_SET(fileno(stdin), &rfds);


        /* Wait up to five seconds. */

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        retval = select(fd_read + 1, &rfds, NULL, NULL, &tv);
        /* Don't rely on the value of tv now! */

        if (retval > 0){
            if (FD_ISSET(fd_read, &rfds)){
                if (read(fd_read, buffer, sizeof(buffer)) == -1){
                    handle_error("read");
                }
                printf("Mensaje recibido: %s\n", buffer);
            }

            if (FD_ISSET(fileno(stdin), &rfds)) {
                printf("Mensaje a enviar: ");

                if (fgets(buffer, sizeof(buffer), stdin)) {
                    buffer[strcspn(buffer, "\n")] = '\0';
                    if ((write(fd_write, buffer, strlen(buffer) + 1) == -1)){
                        handle_error("write");
                    }
                }
            }
        }
    }

    return 0;
}