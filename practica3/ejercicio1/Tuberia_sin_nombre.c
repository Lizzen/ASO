#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    if (argc < 5){
        printf("Faltan argumentos: ./programa comando1 argumento 1 | comando2 argumento2\n");
        exit(EXIT_FAILURE);
    }

    int pipe_fd[2];

    //Crea tubería sin nombre
    if (pipe(pipe_fd) == -1){
        perror("pipe: Error al crear la tubería sin nombre.\n");
        exit(EXIT_FAILURE);
    }

    //Crea proceso primer hijo
    pid_t pid = fork();
    
    if (pid == -1){
        perror("fork: Error al crear hijo1.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){

        //Cierra extremo de lectura
        if (close(pipe_fd[0]) == -1){
            perror("close: Error al cerrar el extremo de lectura.\n");
            exit(EXIT_FAILURE);
        }

        //Duplica la salida estandar con el extremo de escritura de la tubería
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1){
            perror("dup2: Error al duplicar la salida estandar con el extremo de escritura.\n");
            exit(EXIT_FAILURE);
        }

        //Cierra extremo de escritura que no vamos a usar 
        close(pipe_fd[1]);

        //Ejecuta comando1 argumento1
        if (execlp(argv[1], argv[2], NULL) == -1) {
            perror("execlp: Error al ejecutar ls -l.\n");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid2 = fork();
    
    if (pid2 == -1){
        perror("fork: Error al crear hijo2.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid2 == 0){

        //Cierra extremo de escritura
        if (close(pipe_fd[1]) == -1){
            perror("close: Error al cerrar STDIN_FILENO.\n");
            exit(EXIT_FAILURE);
        }

        //Duplica la salida estandar con el extremo de escritura de la tubería
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1){
            perror("dup2: Error al duplicar la salida estandar con el extremo de escritura.\n");
            exit(EXIT_FAILURE);
        }

        //Cierra extremo de lectura
        close(pipe_fd[0]);

        //Ejecuta comando2 argumento2
        if (execlp(argv[3], argv[4], NULL) == -1) {
            perror("execlp: Error al ejecutar wc -l.\n");
            exit(EXIT_FAILURE);
        }
    }

    //Cierra extremo de lectura y escritura que no vamos a usar en el proceso padre
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    waitpid(pid, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}