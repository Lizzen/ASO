#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int signum) {
    printf("Señal SIGINT recibida\n");
}

void sigtstp_handler(int signum) {
    printf("Señal SIGTSTP recibida\n");
    signal(SIGTSTP, SIG_DFL); // Desbloquea SIGTSTP para que se pueda suspender el proceso
}

int main() {
    // Bloquea SIGINT y SIGTSTP
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTSTP);
    sigprocmask(SIG_BLOCK, &set, NULL);

    // Obtiene el número de segundos de la variable de entorno
    char *sleep_secs_str = getenv("SLEEP_SECS");
    if (sleep_secs_str == NULL) {
        printf("No se ha especificado la variable SLEEP_SECS\n");
        exit(EXIT_FAILURE);
    }
    int sleep_secs = atoi(sleep_secs_str);

    // Configura los manejadores de señales
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);

    printf("El proceso se suspenderá durante %d segundos...\n", sleep_secs);
    sleep(sleep_secs);

    // Comprueba si se recibieron las señales
    sigset_t pending_set;
    sigpending(&pending_set);
    if (sigismember(&pending_set, SIGINT)) {
        printf("El proceso recibió la señal SIGINT durante la suspensión\n");
    }
    if (sigismember(&pending_set, SIGTSTP)) {
        printf("El proceso recibió la señal SIGTSTP durante la suspensión\n");
    }

    printf("Fin del programa\n");

    return 0;
}
