#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Faltan argumentos: ./programa examenmayo manuel\n");
        exit(EXIT_FAILURE);
    }

    int pipe_fd[2];

    //Crea tubería sin nombre
    if (pipe(pipe_fd) == -1){
        perror("pipe: Error al crear la tubería sin nombre.\n");
        exit(EXIT_FAILURE);
    }

    //Crea proceso primer hijo
    /*pid_t pid = fork();
    
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
        if (execlp("cat", argv[1], NULL) == -1) {
            perror("execlp: Error al ejecutar cat.\n");
            exit(EXIT_FAILURE);
        }
    }*/


    pid_t pid2 = fork();
    
    if (pid2 == -1){
        perror("fork: Error al crear hijo2.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid2 == 0){

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

        //Cierra extremo de escritura
        close(pipe_fd[1]);

        //Ejecuta comando2 argumento2
        if (execlp("grep", "-e", argv[2], argv[1], NULL) == -1) {
            perror("execlp: Error al ejecutar grep.\n");
            exit(EXIT_FAILURE);
        }
    }

    /*pid_t pid3 = fork();
    
    if (pid3 == -1){
        perror("fork: Error al crear hijo2.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid3 == 0){
        //Cierra extremo de escritura
        if (close(pipe_fd[0]) == -1){
            perror("close: Error al cerrar STDIN_FILENO.\n");
            exit(EXIT_FAILURE);
        }

        //Duplica la salida estandar con el extremo de escritura de la tubería
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1){
            perror("dup2: Error al duplicar la salida estandar con el extremo de escritura.\n");
            exit(EXIT_FAILURE);
        }

        //Cierra extremo de lectura
        close(pipe_fd[1]);

        //Ejecuta comando2 argumento2
        if (execlp("cut", "cut", "-d", " ", "-f", "2", NULL) == -1) {
            perror("execlp: Error al ejecutar cut.\n");
            exit(EXIT_FAILURE);
        }
    }*/


    //Cierra extremo de lectura y escritura que no vamos a usar en el proceso padre
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    //waitpid(pid, NULL, 0);
    waitpid(pid2, NULL, 0);
    //waitpid(pid3, NULL, 0);

    return 0;
}