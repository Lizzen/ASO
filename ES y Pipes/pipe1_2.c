#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define PIPE_NAME "~/tuberia"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s message\n", argv[0]);
        exit(1);
    }

    char* message = argv[1];

    if (mkfifo(PIPE_NAME, 0666) == -1){
        error("Error al crear la tuberia");
        exit(EXIT_FAILURE);
    }
    int pipe_fd = open(PIPE_NAME, O_WRONLY);
    write(pipe_fd, message, strlen(message));
    close(pipe_fd);

    return 0;
}