#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>

int main(){
    char *pipe0= "tuberia1";
    char *pipe1= "tuberia2";

    // Crea una tubería FIFO con permisos de lectura, escritura y ejecución rwx-rwx-rwx
    if (mkfifo(pipe0, 0777) == -1){
        perror("mkfifo: Error al crear una tubería FIFO con permisos de lectura y escritura.\n");
        exit(EXIT_FAILURE);
    }

    if (mkfifo(pipe1, 0777) == -1){
        perror("mkfifo: Error al crear una tubería FIFO con permisos de lectura y escritura.\n");
        exit(EXIT_FAILURE);
    }

    int fd[2];
    fd[0] = open(pipe0, O_WRONLY | O_APPEND | O_RDONLY |O_NONBLOCK);
    fd[1] = open(pipe1, O_WRONLY | O_APPEND | O_RDONLY |O_NONBLOCK);

    // Calcula el descriptor máximo para la función select()
    int maxfd = (fd[0] > fd[1]) ? fd[0] : fd[1];
    fd_set rfds;
    char buffer[256];
    while (1){
        FD_ZERO(&rfds);
        FD_SET(fd[0], &rfds);
        FD_SET(fd[1], &rfds);

        if(select(maxfd + 1, &rfds, NULL, NULL, NULL)){
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(fd[0], &rfds)){
            printf("Datos leidos de la tubería1:\n");
            ssize_t n = read(fd[0], buffer, sizeof(buffer));
            if (n > 0){
                write(STDOUT_FILENO, buffer, n);
            }
            printf("\n");
        }

        if (FD_ISSET(fd[1], &rfds)){
            printf("Datos leidos de la tubería2:\n");
            ssize_t n = read(fd[1], buffer, sizeof(buffer));
            if (n > 0){
                write(STDOUT_FILENO, buffer, n);
            }
            printf("\n");
        }

        if (FD_ISSET(STDIN_FILENO, &rfds)) {
            printf("Fin del programa\n");
            exit(EXIT_SUCCESS);
        }
    }

    close(fd);

    return 0;
}