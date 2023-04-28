/* shell.c -- Shell elemental. Solo admite jobs con una orden */

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

// Variables globales
struct listaJobs listaJobs = {NULL, NULL};

// Funciones auxiliares
void manejadorSIGINT(int sig) {
    // Enviar señal SIGINT al proceso en foreground
    if (listaJobs.fg != NULL) {
        kill(-listaJobs.fg->pgrp, SIGINT);
    }
}

void manejadorSIGQUIT(int sig) {
    // Enviar señal SIGQUIT al proceso en foreground
    if (listaJobs.fg!= NULL) {
        kill(-listaJobs.fg->pgrp, SIGQUIT);
    }
}



// Programa principal
int main(int argc, char **argv) {

    char orden[LONG_MAX_ORDEN + 1];
    char *otraOrden = NULL;
    struct job JobNuevo;
    int esBg=0;

    // Procesamiento argumentos de entrada (versión simplificada)
    if (argc > 1) {
        fprintf(stderr, "Uso: Shell");
        exit(EXIT_FAILURE);
    }

    // Ignorar la señal SIGTTOU, capturar SIGINT, SIGQUIT...
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGTTOU, &sa, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = manejadorSIGINT;
    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = manejadorSIGQUIT;
    if (sigaction(SIGQUIT, &sa, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }



    // Repetir
    while (1) {
    
    // Si no hay job_en_foreground
    if (listaJobs.fg != NULL){
	    // Comprobar finalización de jobs
        compruebaJobs(&listaJobs);
        
	    // Leer ordenes
	    if (!otraOrden) {
            if (leeOrden(stdin, orden)) break;
            otraOrden = orden;
        }

	    // Si la orden no es vacia, analizarla y ejecutarla
	    if (!analizaOrden(&otraOrden,&JobNuevo,&esBg) && JobNuevo.numProgs) {
		    ejecutaOrden(&JobNuevo,&listaJobs,esBg);
        }
    }
    // (Else) Si existe job en foreground
    else{
	    // Esperar a que acabe el proceso que se encuentra en foreground
        pid_t pid = listaJobs.fg->progs[0].pid;
        int status;
        if (waitpid(pid, &status, WUNTRACED) < 0) {
                perror("waitpid");
                exit(EXIT_FAILURE);
        }
	    // Recuperar el terminal de control
        tcsetpgrp(STDIN_FILENO, getpid());
	    // Si parada_desde_terminal
        if (WIFSTOPPED(status)) {
	        // Informar de la parada
            printf("Job %d parado\n", listaJobs.fg->jobId);
	        // Actualizar el estado del job y la lista
            listaJobs.fg->runningProgs = 1;
            listaJobs.fg->progs[0].pid = status;
            listaJobs.fg->pgrp = listaJobs.fg->progs[0].pid;
        }
	    // (Else) si no
        else{
	        // Eliminar el job de la lista
            eliminaJob(&listaJobs, listaJobs.fg->progs[0].pid, 0);
        }
    }
    }

   // Salir del programa (codigo error)
   exit(EXIT_FAILURE);
}

