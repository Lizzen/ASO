#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

    if (argc < 3){
        printf("Faltan argumentos: ./programa filename string\n");
        exit(EXIT_FAILURE);
    }

    int pipe_fd1[2], pipe_fd2[2];

    char *filename = argv[1];
    char *string_buscado = argv[2];

    //Crea tubería sin nombre
    if (pipe(pipe_fd1) == -1){
        perror("pipe: Error al crear la tubería sin nombre.\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipe_fd2) == -1){
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
        if (close(pipe_fd1[0]) == -1){
            perror("close: Error al cerrar el extremo de lectura.\n");
            exit(EXIT_FAILURE);
        }

        //Duplica la salida estandar con el extremo de escritura de la tubería
        if (dup2(pipe_fd1[1], STDOUT_FILENO) == -1){
            perror("dup2: Error al duplicar la salida estandar con el extremo de escritura.\n");
            exit(EXIT_FAILURE);
        }

        //Cierra extremo de escritura que no vamos a usar 
        close(pipe_fd1[1]);

        //Ejecuta comando1 argumento1
        if (execlp("cat", "cat", filename, NULL) == -1) {
            perror("execlp: Error al ejecutar cat.\n");
            exit(EXIT_FAILURE);
        }
        exit(1);
    }


    pid_t pid2 = fork();
    
    if (pid2 == -1){
        perror("fork: Error al crear hijo2.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid2 == 0){

        //Cierra extremo de escritura
        close(pipe_fd1[1]);

        //Duplica la salida estandar con el extremo de lectura de la tubería
        dup2(pipe_fd1[0], STDIN_FILENO);

        //Cierra extremo de lectura
        close(pipe_fd1[0]);

        //Cierra extremo de lectura
        close(pipe_fd2[0]);

        //Duplica la salida estandar con el extremo de escritura de la tubería
        dup2(pipe_fd2[1], STDOUT_FILENO);

        //Cierra extremo de escritura
        close(pipe_fd2[1]);

        //Ejecuta comando2 argumento2
        if (execlp("grep", "grep", string_buscado, NULL) == -1) {
            perror("execlp: Error al ejecutar grep.\n");
            exit(EXIT_FAILURE);
        }
        exit(1);
    }

    pid_t pid3 = fork();
    
    if (pid3 == -1){
        perror("fork: Error al crear hijo2.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid3 == 0){
        //Cierra extremo de escritura
        if (close(pipe_fd2[1]) == -1){
            perror("close: Error al cerrar STDIN_FILENO.\n");
            exit(EXIT_FAILURE);
        }

        //Duplica la salida estandar con el extremo de lectura de la tubería
        if (dup2(pipe_fd2[0], STDIN_FILENO) == -1){
            perror("dup2: Error al duplicar la salida estandar con el extremo de escritura.\n");
            exit(EXIT_FAILURE);
        }

        //Cierra extremo de lectura
        close(pipe_fd2[0]);

        //Ejecuta comando2 argumento2
        if (execlp("cut", "cut", "-d", " ", "-f", "2", NULL) == -1) {
            perror("execlp: Error al ejecutar cut.\n");
            exit(EXIT_FAILURE);
        }
        exit(1);
    }


    //Cierra extremo de lectura y escritura que no vamos a usar en el proceso padre
    close(pipe_fd1[0]);
    close(pipe_fd1[1]);
    close(pipe_fd2[0]);
    close(pipe_fd2[1]);


    waitpid(pid, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    return 0;
}