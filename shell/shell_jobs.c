/* shell_jobs.c -- rutinas relativas a jobs */ 

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

// Libera la memoria dinamica consumida por los campos del job
void liberaJob(struct job *job) {
    // Liberar los recursos consumidos por la entrada
    if (job->texto) free(job->texto);
    if (job->ordenBuf) free(job->ordenBuf);
    if (job->progs[0].argv) free(job->progs[0].argv);
    if (job->progs[0].argv) free(job->progs);
}

// Elimina el job de lista y actualiza el puntero fg si procede
void eliminaJob(struct listaJobs *listaJobs, int pid, int esBg) {
    struct job *j,*k;
    
    // Localizar el job dentro de la lista
    for (j=listaJobs->primero,k=NULL;j;k=j,j=j->sigue)
         if (j->progs[0].pid == pid) break;
	
    // Si el job esta en la lista 
    if (j) {
	 
	 // Si se trata de un job fg
	 if (esBg)
	    // Vaciar el puntero fg
	    listaJobs->fg=NULL;
	 // Sino
	 else
	    // Informar de la terminación
	    fprintf(stdout,"[%d] Done\t%d\t%s\n",j->jobId,j->progs[0].pid,j->texto);   
	    
	 // Eliminar el job de la lista
	 if (k)
	    k->sigue=j->sigue;
	 else
	    listaJobs->primero=j->sigue;
         
	 // Liberar recursos
	 liberaJob(j);free(j);
	 
    // Si el job no esta en la lista	    
    } else {
         // Mostrar mensaje de error
         fprintf(stderr,"Error: el proceso %d no es un hijo\n",pid);
    }
}

// Inserta un nuevo job en la lista
void insertaJob(struct listaJobs *listaJobs, struct job *job, int esBg)  {
    struct job *j,*k;
    int jobId = 0;
    
    // Localizar final de la listay último jobID
    for (j=listaJobs->primero,k=NULL;j;k=j,j=j->sigue)
	jobId = j->jobId;
    
    // Insertar
    if (k)
       k->sigue = job;
    else
       listaJobs->primero=job;
    
    // Actualizar jobId 
    job->jobId = jobId + 1;     
}

// Localiza un job en la lista mediante su jobID
struct job *buscaJob(struct listaJobs *listaJobs, int jobID)  {
    struct job *j;
    
    // Localizar final de la listay último jobID
    for (j=listaJobs->primero;j;j=j->sigue)
	if (j->jobId == jobID) return j;
    
    // Si no esta en la lista devolver puntero nulo
    return NULL; 
}

// Comprueba si hay hijos finalizados y si los hay los elimina de la lista
void compruebaJobs(struct listaJobs *listaJobs) {
   pid_t pid;
   
   // Mientras haya hijos finalizados
   while ((pid=waitpid(-1,NULL,WNOHANG))>0)
      // Informar de su terminación y eliminarlos de la lista
      eliminaJob(listaJobs,pid,0);
   
      /* Simplificaciones: 
          1) Utilizamos la misma función para eliminar fg y bg
          2) Se omite el chequeo de errores
      */
}
