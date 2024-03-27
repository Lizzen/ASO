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
        unlink(pipe0); unlink(pipe1);
        exit(EXIT_FAILURE);
    }

    if (mkfifo(pipe1, 0777) == -1){
        perror("mkfifo: Error al crear una tubería FIFO con permisos de lectura y escritura.\n");
        exit(EXIT_FAILURE);
    }


    int fd[2];
    fd[0] = open(pipe0, O_NONBLOCK);
    fd[1] = open(pipe1, O_NONBLOCK);

    // Calcula el descriptor máximo para la función select()
    int maxfd = (fd[0] > fd[1]) ? fd[0] : fd[1];
    fd_set rfds;
    char buffer[256];
    int n1 = 0, n2 = 0;
    while (n1 == 0 || n2 == 0){
        FD_ZERO(&rfds);
        FD_SET(fd[0], &rfds);
        FD_SET(fd[1], &rfds);

        if(select(maxfd + 1, &rfds, NULL, NULL, NULL) == -1){
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(fd[0], &rfds) && n1 == 0){
            n1 = read(fd[0], buffer, sizeof(buffer));
            printf("Datos leidos de la tubería1:");
            if (n1 > 0){
                write(STDOUT_FILENO, buffer, n1);
            }
            printf("\n");
        }

        if (FD_ISSET(fd[1], &rfds) && n2 == 0){
            n2 = read(fd[1], buffer, sizeof(buffer));
            printf("Datos leidos de la tubería2:");
            if (n2 > 0){
                write(STDOUT_FILENO, buffer, n2);
            }
            printf("\n");
        }
    }

    close(fd);

    return 0;
}