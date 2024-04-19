#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXMSJ 256

volatile sig_atomic_t flag = 0;

void handler(int signum) {
    if (signum == SIGUSR2 || signum == SIGUSR1 || signum == SIGTERM) {
        flag = 1;
    }
}

int main(int argc, char *argv[]){
    // Inicialización de manejador de señales
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    
    // Crea o abre el archivo mailbox
    int mailbox = open("mailbox.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (mailbox == -1) {
        perror("mailbox: Error al abrir el archivo mailbox");
        exit(EXIT_FAILURE);
    }


    pid_t pid = fork();

    if (pid == -1){
        perror("fork: Error al crear el proceso hijo.\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0){
        // Proceso Hijo

        char mensaje[MAXMSJ];
        while (1) {
            // Espera señal SIGUSR1 del padre
            while (!flag) {
                sleep(1); 
            }
            flag = 0;

            // Lee mensaje del buzón
            lseek(mailbox, 0, SEEK_SET);
            ssize_t bytes_read = read(mailbox, mensaje, MAXMSJ);
            if (bytes_read == -1) {
                perror("mailbox: Error al leer el archivo mailbox.\n");
                exit(EXIT_FAILURE);
            }
            mensaje[bytes_read] = '\0';

            // Imprime mensaje
            printf("Mensaje recibido: %s", mensaje);

            // Envía señal SIGUSR2 al padre
            kill(getppid(), SIGUSR2);

            // Trunca (limpia) el archivo cada vez que se lee
            ftruncate(mailbox, 0);
        }
    }
    else {
        // Proceso padre

        char mensaje[MAXMSJ];
        while (1) {
            // Lee mensaje de entrada estándar
            printf("Inserte un mensaje (EOF para terminar): ");
            if (fgets(mensaje, MAXMSJ, stdin) == NULL) {
                break;  // EOF
            }

            // Escribe mensaje en el buzón
            lseek(mailbox, 0, SEEK_SET);
            ssize_t bytes_written = write(mailbox, mensaje, strlen(mensaje));
            if (bytes_written == -1) {
                perror("mailbox: Error al escribir en el archivo mailbox");
                exit(EXIT_FAILURE);
            }

            // Envía señal SIGUSR1 al hijo
            kill(getpid() + 1, SIGUSR1);

            // Espera señal SIGUSR2 del hijo
            while (!flag) {
                sleep(1); 
            }
            flag = 0;
        }

        // Envía la señal SIGTERM al hijo 
        close(mailbox);
        kill(pid, SIGTERM);
    }

    return 0;
}