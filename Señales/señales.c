// David Ferreras Díaz y Alejandro Zamorano Méndez
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int periodo = 0;
int tiempo = 0;

// Manejador de señal SIGTERM
void sigterm() {
    printf("\nSegundos transcurridos: %d\n", tiempo);
    exit(0);
}

// Manejador de señal SIGALRM
void sigalrm() {
    tiempo += periodo;
    alarm(periodo);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No hay suficientes argumentos\n");
        exit(1);
    }

    // Obtener el periodo de actualización
    periodo = atoi(argv[1]);
    if (periodo <= 0) {
        printf("El argumento segundos debe ser un número entero positivo\n");
        exit(1);
    }

    // Ignorar la señal SIGINT
    if (signal(SIGINT, SIG_IGN) == SIG_ERR){
        printf("Error al capturar la señal SIGINT\n");
        exit(EXIT_FAILURE);
    }

    // Asignar el manejador de señal SIGTERM
    if (signal(SIGTERM, sigterm) == SIG_ERR) {
        printf("Error al capturar la señal SIGTERM\n");
        exit(EXIT_FAILURE);
    }

    // Asignar el manejador de señal SIGALRM
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
        pause();    // Esperar por una señal
    }

    return 0;
}

