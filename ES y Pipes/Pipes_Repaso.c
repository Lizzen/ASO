#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_MSG_LEN 100
#define MAX_MSG_NUM 10

int main()
{
    int pipe1[2], pipe2[2]; // tuberías para padre e hijo 
    pid_t pid;
    int i, read_len, msg_count = 0;
    char buffer[MAX_MSG_LEN];

    // crea las tuberías
    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        perror("Error en pipe");
        exit(EXIT_FAILURE);
    }

    // crea un proceso hijo
    if ((pid = fork()) < 0) {
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // código del proceso hijo
        close(pipe1[1]); // cierra extremo de escritura de la tubería 1
        close(pipe2[0]); // cierra extremo de lectura de la tubería 2

        while (1) {
            read_len = read(pipe1[0], buffer, MAX_MSG_LEN);
            if (read_len < 0) {
                perror("Error en read");
                exit(EXIT_FAILURE);
            }
            if (read_len == 0) { // el padre ha cerrado la tubería
                break;
            }
            buffer[read_len] = '\0';
            printf("Proceso hijo recibió: %s", buffer);
            sleep(1);
            write(pipe2[1], "l", 1); // envia confirmación de que se procesó el mensaje
            if (++msg_count >= MAX_MSG_NUM) {
                write(pipe2[1], "q", 1); // envia señal de salida
                break;
            }
        }

        close(pipe1[0]); // cierra extremo de lectura de la tubería 1
        close(pipe2[1]); // cierra extremo de escritura de la tubería 2
        exit(EXIT_SUCCESS);
    }
    else {  // código del proceso padre
        close(pipe1[0]); // cierra extremo de lectura de la tubería 1
        close(pipe2[1]); // cierra extremo de escritura de la tubería 2

        while (1) {
            printf("Introduzca un mensaje: ");
            fgets(buffer, MAX_MSG_LEN, stdin);
            write(pipe1[1], buffer, strlen(buffer));
            read_len = read(pipe2[0], buffer, 1);
            if (read_len < 0) {
                perror("Error en read");
                exit(EXIT_FAILURE);
            }
            if (buffer[0] == 'q') { // el hijo ha terminado
                break;
            }
            printf("Proceso padre recibió confirmación del hijo\n");
        }

        close(pipe1[1]); // cierra extremo de escritura de la tubería 1
        close(pipe2[0]); // cierra extremo de lectura de la tubería 2

        wait(NULL); // espera a que el hijo termine
        printf("Proceso hijo terminó, proceso padre también\n");
        exit(EXIT_SUCCESS);
    }
}
