// David Ferreras Díaz y Alejandro Zamorano Méndez
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Faltan argumentos: %s comando1 argumento1 | comando2 argumento2\n", argv[0]);
        exit(1);
    }
    
    // Crear tubería sin nombre
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    // Crear proceso hijo
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // Proceso hijo
        // Cerrar extremo de escritura
        if (close(fd[1]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        
        // Duplicar entrada estándar con extremo de lectura de la tubería
        if (dup2(fd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        // Ejecutar comando2 argumento2
        if (execlp(argv[3], argv[3], argv[4], NULL) == -1) {
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    } else {  // Proceso padre
        // Cerrar extremo de lectura
        if (close(fd[0]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        
        // Duplicar salida estándar con extremo de escritura de la tubería
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        // Ejecutar comando1 argumento1
        if (execlp(argv[1], argv[1], argv[2], NULL) == -1) {
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }
    
    // Esperar a que el proceso hijo termine
    if (wait(NULL) == -1) {
        perror("wait");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
