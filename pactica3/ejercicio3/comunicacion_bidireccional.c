#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define READ_PIPE 0
#define WRITE_PIPE 1

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
        close(pipe_hijo[READ_PIPE]);
        close(pipe_padre[WRITE_PIPE]);

        char buffer[256];

        if (read(pipe_padre[READ_PIPE], buffer, sizeof(buffer) == -1){
            perror("read: Error al leer del pipe.\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; buffer[i]; ++i){
            buffer[i] = toupper(buffer[i]);
        }

        printf("Mensaje recibido del padre:\n %s\n", buffer);
        sleep(1); 
        if (write(pipe_hijo[WRITE_PIPE], 'n', 1) == -1){
            perror("write: Error al escribir en el pipe del hijo_padre.\n");
            exit(EXIT_FAILURE);
        }
    }
    else{
        // Proceso padre
        close(pipe_padre[READ_PIPE]);
        close(pipe_hijo[WRITE_PIPE]);
        
        char buffer[256];
        char hijo;
        while (hijo != 'q'){
            printf("Inserta un mensaje: ");
            fgets(buffer, sizeof(buffer), stdin);
            write(pipe_padre[WRITE_PIPE]);
            read(pipe_hijo[READ_PIPE], &hijo, sizeof(hijo));
            wait(NULL); // Wait for child to finish
        }
        close(pipe_padre[WRITE_PIPE]);
        close(pipe_hijo[READ_PIPE]);      
    }
}