/* shell.h -- cabecera para shell.c */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#define LONG_MAX_ORDEN  250     // longitud max de una cadena de orden
#define MAX_ARGS	32	// numero maximo de argumentos

// Tipos de datos
struct ProgHijo {
    pid_t pid;			// 0 si ya termino
    char **argv;		// nombre y argumentos del programa
};

struct job {
    int jobId;			// numero de job
    char *texto;		// cadena de orden del job
    char *ordenBuf;		// cadenas con los argumentos
    int numProgs;		// numero de programas que integran el job (1)
    int runningProgs;		// programas que quedan por acabar (1 o 0)
    pid_t pgrp;			// ID de grupo de procesos en job
    struct ProgHijo *progs; 	// programas que integran el job (progs[0])
    struct job *sigue;		// enlace al siguiente job activo
};

struct listaJobs {
    struct job *primero;
    struct job *fg;
};


// Prototipos procesamiento ordenes
int leeOrden(FILE *fuente, char *orden);
int analizaOrden(char **ordenPtr, struct job *job, int *esBg);
void ejecutaOrden(struct job *job, struct listaJobs *listaJobs, int esBg);

// Prototipos gestion de jobs
void liberaJob(struct job *job);
void eliminaJob(struct listaJobs *listaJobs, int pid, int esBg);
void insertaJob(struct listaJobs *listaJobs, struct job *job, int esBg);
struct job *buscaJob(struct listaJobs *listaJobs, int jobID);
void compruebaJobs(struct listaJobs *listaJobs);

