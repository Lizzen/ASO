/*Modifica el programa localtime.c para que muestre la fecha y hora actuales del sistema, de acuerdo con el patrón
Hoy es Viernes, 11:40.*/

#include <sys/types.h>
#include <time.h>
#include <stdio.h>

//consultar time(2), localtime(3), strftime(3)

int main()
{
    time_t now = time(NULL);
    char *hoy = ctime(&now);

/* Obtencion del tiempo actual (time_t) */
    printf("Hoy es (time_t): %s", hoy);

/* Obtencion de la estructura  (struct tm) */
    struct tm *localTime = localtime(&now);

/* Formatear la fecha en una cadena con el patron ejemplo
   "Hoy es Viernes 11:40" */
    char formateo[80]; 
    strftime(formateo, sizeof(formateo), "Hoy es %A, %H:%M\n", localTime);
    
/* Mostrar la cadena resultante */    
    printf("%s\n", formateo);
    
    return 1;
}    
