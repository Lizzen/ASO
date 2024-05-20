#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <string.h>

#define READ_PIPE 0
#define WRITE_PIPE 1

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int pipe_padre_hijo[2], pipe_hijo_padre[2];

    if (pipe(pipe_padre_hijo) == -1) {
        handle_error("pipe padre_hijo");
    }

    if (pipe(pipe_hijo_padre) == -1) {
        handle_error("pipe hijo_padre");
    }

    pid_t pid = fork();

    if (pid == -1) {
        handle_error("fork");
    } else if (pid == 0) {
        // Proceso hijo
        close(pipe_padre_hijo[WRITE_PIPE]);
        close(pipe_hijo_padre[READ_PIPE]);

        char buffer[256];
        int mensajes_recibidos = 0;

        while (1) {
            // Lee el mensaje del padre
            ssize_t bytes_leidos = read(pipe_padre_hijo[READ_PIPE], buffer, sizeof(buffer) - 1);
            if (bytes_leidos == -1) {
                handle_error("read hijo");
            }
            buffer[bytes_leidos] = '\0';
            
            // Lo transforma a mayusculas
            for (int i = 0; buffer[i]; ++i) {
                buffer[i] = toupper(buffer[i]);
            }

            // Imprime el mensaje y espera
            printf("Mensaje recibido del padre: %s", buffer);
            fflush(stdout);
            sleep(1);

            // Si se han recibido 10 mensajes envia q para que el programa termine, si no n
            mensajes_recibidos++;
            char respuesta = (mensajes_recibidos < 10) ? 'n' : 'q';

            if (write(pipe_hijo_padre[WRITE_PIPE], &respuesta, 1) == -1) {
                handle_error("write hijo");
            }

            if (respuesta == 'q') {
                break;
            }
        }

        close(pipe_padre_hijo[READ_PIPE]);
        close(pipe_hijo_padre[WRITE_PIPE]);
    } else {
        // Proceso padre
        close(pipe_padre_hijo[READ_PIPE]);
        close(pipe_hijo_padre[WRITE_PIPE]);

        char buffer[256];
        char hijo_respuesta;

        while (1) {
            printf("Inserta un mensaje: ");
            fflush(stdout);

            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                handle_error("fgets padre");
            }

            if (write(pipe_padre_hijo[WRITE_PIPE], buffer, strlen(buffer)) == -1) {
                handle_error("write padre");
            }

            if (read(pipe_hijo_padre[READ_PIPE], &hijo_respuesta, 1) == -1) {
                handle_error("read padre");
            }

            if (hijo_respuesta == 'q') {
                break;
            }
        }

        close(pipe_padre_hijo[WRITE_PIPE]);
        close(pipe_hijo_padre[READ_PIPE]);

        // Se espera a que termine el hijo
        wait(NULL); 
    }

    return 0;
}
