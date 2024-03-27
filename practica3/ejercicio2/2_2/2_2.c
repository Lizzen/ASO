#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main (int argc, char *argv[]){
    char *pipe= "tuberia1";

    // Crea una tubería FIFO con permisos de lectura, escritura y ejecución rwx-rwx-rwx
    if (mkfifo(pipe, 0777) == -1){
        perror("mkfifo: Error al crear una tubería FIFO con permisos de lectura y escritura.\n");
        unlink(pipe);
        exit(EXIT_FAILURE);
    }

    int fd = open(pipe, O_RDWR | O_NONBLOCK);

    char *msg = argv[1];
    char *cat[256];
    if (write(fd, msg, strlen(msg)) == -1){
        perror("write: Error al escribir en el pipe.\n");
        unlink(pipe);
        exit(EXIT_FAILURE);
    }
    else {
        read(fd, cat, sizeof(cat));
        printf("cat < tuberia1: %s\n", cat);
    }

    close(fd);
    return 0;
}

// cat < tuberia1