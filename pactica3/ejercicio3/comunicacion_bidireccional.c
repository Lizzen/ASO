#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(){
    int pipe_padre[2], pipe_hijo[2];

    if(mkfifo(pipe_padre, 0777) == -1){
        perror("mkfifo: Error al crear las tuberías que conectan el padre con el hijo.\n");
        exit(EXIT_FAILURE); 
    }

    if(mkfifo(pipe_hijo, 0777) == -1){
    perror("mkfifo: Error al crear las tuberías que conectan el hijo con el padre.\n");
    exit(EXIT_FAILURE); 
    }

    pid_t pid = fork();

    if (pid == -1){
        perror("fork: Error al crear el proceso hijo.\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0){
        // Proceso hijo
        
    }
    else{
        // Proceso padre
        
    }
 }