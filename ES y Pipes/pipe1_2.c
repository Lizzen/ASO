// David Ferreras Díaz y Alejandro Zamorano Méndez
/*Escribir un programa que abra la tubería con nombre anterior (~/tuberia) en modo solo
escritura, y escriba en ella el primer argumento del programa. En otro terminal, leer de la tubería
usando un comando adecuado.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define PIPE_NAME "~/tuberia"

int main(int argc, char* argv[]) {
    // Verificar si se ha pasado el mensaje como argumento
    if (argc < 2) {
        printf("Faltan argumentos: message\n");
        exit(1);
    }

    char* message = argv[1];

    // Crear la tubería FIFO si no existe
    if (mkfifo(PIPE_NAME, 0666) == -1){
        error("Error al crear la tuberia");
        exit(EXIT_FAILURE);
    }
    // Abrir la tubería FIFO en modo escritura
    int pipe_fd = open(PIPE_NAME, O_WRONLY);
    // Escribir el mensaje en la tubería
    write(pipe_fd, message, strlen(message));
    // Cerrar la tubería
    close(pipe_fd);

    return 0;
}
