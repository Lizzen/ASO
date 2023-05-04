#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigalrm() {
    longjmp(entorno,1);
}

int main(int argc, char *argv[]) {
    dormir(argv[0]);

    return 0;
}

void dormir(int seg){
    void (*anterior)();
    int alarma_anterior;
    if ( seg <= 0) {
        printf("Error: los segundos no pueden ser menor que 0\n");
        exit(EXIT_FAILURE);
    }

    anterior = signal(SIGALRM, sigalrm);

    alarm(periodo);

    if (setjmp(entorno) == 0){
        alarma_anterior = alarm(seg);
        pause();
    }
    else{
        signal(sigalrm, anterior);
        alarm(alarma_anterior);
    } 
}