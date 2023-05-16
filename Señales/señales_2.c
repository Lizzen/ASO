#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int count_sigint = 0;
int count_sigtstp = 0;

void sig_handler(int signum) {
    if (signum == SIGINT) {
        count_sigint++;
        printf("Señal SIGINT recibida %d veces\n", count_sigint);
    } else if (signum == SIGTSTP) {
        count_sigtstp++;
        printf("Señal SIGTSTP recibida %d veces\n", count_sigtstp);
    }
}

int main() {
    // Instalación del manejador de señales
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        printf("No se pudo instalar el manejador de SIGINT\n");
        exit(1);
    }
    if (signal(SIGTSTP, sig_handler) == SIG_ERR) {
        printf("No se pudo instalar el manejador de SIGTSTP\n");
        exit(1);
    }

    int count = 0;
    while (count < 10) {
        printf("Esperando señales...\n");
        sleep(1);
        count++;
    }

    printf("Señal SIGINT recibida %d veces\n", count_sigint);
    printf("Señal SIGTSTP recibida %d veces\n", count_sigtstp);

    return 0;
}
