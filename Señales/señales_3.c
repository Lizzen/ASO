#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*void sigalrm() {
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
}*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF 256

void sigusr1_handler(int signum);
void remove_self(char* filename);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No se ha especificado el tiempo de espera\n");
        exit(EXIT_FAILURE);
    }
    int tiempo = atoi(argv[1]);
    if (tiempo <= 0) {
        printf("El tiempo de espera debe ser mayor que 0\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
        printf("Error al capturar la señal SIGUSR1\n");
        exit(EXIT_FAILURE);
    }

    // Almacenamos el nombre del ejecutable
    char buf[MAX_BUF];
    int len = readlink("/proc/self/exe", buf, MAX_BUF);
    if (len == -1) {
        printf("Error al obtener el nombre del archivo ejecutable\n");
        exit(EXIT_FAILURE);
    }
    buf[len] = '\0';
    printf("El archivo a borrar es %s\n", buf);

    // Configuramos el temporizador
    alarm(tiempo);

    while (1) {
        pause();
    }

    return 0;
}

void sigusr1_handler(int signum) {
    printf("Se ha recibido la señal SIGUSR1. El borrado del archivo se ha detenido.\n");
    exit(EXIT_SUCCESS);
}

void remove_self(char* filename) {
    if (remove(filename) == -1) {
        printf("Error al borrar el archivo\n");
        exit(EXIT_FAILURE);
    }
    printf("El archivo %s ha sido borrado\n", filename);
    exit(EXIT_SUCCESS);
}

void alarm_handler(int signum) {
    printf("El temporizador ha expirado. Borrando el archivo...\n");
    remove_self(buf);
}
