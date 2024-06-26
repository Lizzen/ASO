
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


// Manejador de señal SIGTERM
void sigterm() {
    printf("Atrapando la señal SIGUSR1 o SIGTSTP.\n");
    exit(0);
}

// Manejador de señal SIGALRM
void sigalrm() {
    printf("Ignorando SIGUSR1 y SIGTSTP.\n");
    signal(SIGUSR1, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}

void ignorar_señales(){
    for (int i = 0; i < _NSIG; ++i){
        if (i != SIGALRM){
            signal(i, SIG_IGN);
        }
    }
}

void compute(int iter){
    printf("Ejecutando iteración %d...\n", iter);
    sleep(2);
}

int main(int argc, char *argv[]) {
    struct sigaction sa;
    sa.sa_handler = sigterm;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);

    ignorar_señales();
    signal(SIGALRM, sigalrm);

    int iter=0;
    while (1) {
        compute(iter);
        printf("Completada iteracion %d. ",iter);
        printf("Tienes 5 segundos para interrumpir ejecución...\n"),
        signal(SIGUSR1, sigterm);
        signal(SIGTSTP, sigterm);
        alarm(5);
        sleep(5);
        iter++;
    }

    return 0;
}

