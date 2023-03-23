#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#

int periodo = 0;
int tiempo = 0;

void sigterm() {
    printf("\nSegundos transcurridos: %d\n", tiempo);
    exit(0);
}

void sigalrm() {
    tiempo += periodo;
    alarm(periodo);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No hay suficientes argumentos\n");
        exit(1);
    }

    periodo = atoi(argv[1]);
    if (periodo <= 0) {
        printf("El argumento segundos debe ser un número entero positivo\n");
        exit(1);
    }

    if (signal(SIGINT, SIG_IGN) == SIG_ERR){
        printf("Error al capturar la señal SIGINT\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGTERM, sigterm) == SIG_ERR) {
        printf("Error al capturar la señal SIGTERM\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGALRM, sigalrm) == SIG_ERR) {
        printf("Error al capturar la señal SIGALRM\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid =  getpid();
    alarm(periodo);

    while (1) {
        printf("PID:%d Mensaje en el segundo %d", pid, tiempo);
        if (argc > 2){
            printf(" [%s]\n", argv[2]);
        }
        else {printf("\n");}
        pause();
    }

    return 0;
}

