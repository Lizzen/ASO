#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main (char *argv[]){
    char *pipe= "mituberia";

    // Crea una tubería FIFO con permisos de lectura, escritura y ejecución rwx-rwx-rwx
    if (mkfifo(pipe, 0777) == -1){
        perror("mkfifo: Error al crear una tubería FIFO con permisos de lectura y escritura.\n");
        exit(EXIT_FAILURE);
    }

    int fd = open(pipe, O_WRONLY | O_APPEND | O_NONBLOCK);

    char *msg = argv[1];
    if (write(fd, msg, strlen(msg)) == -1){
        perror("write: Error al escribir en el pipe.\n");
        exit(EXIT_FAILURE);
    }

    close(fd);

    return 0;
}