/*Cuando es necesario obtener la información horaria con precisión de microsegundos se puede usar gettimeofday().
Escribir un programa (gettime.c) que mida cuánto tarda un bucle de 10000 iteraciones. En cada iteración, se
incrementará una variable de tipo int llamada mivar con un valor entero que se debe pasar al programa como
argumento. Si no se pasa este argumento el programa mostrará un mensaje de error y terminará devolviendo el valor 1.
En otro caso el programa mostrará por la salida estándar el valor final de mivar y el tiempo transcurrido, retornando el
programa el valor 0.*/

#include <sys/time.h>
#include <stdio.h>

int main(){
    struct timeval ini, fin;
    
    int i, mivar = 0;
    gettimeofday(&ini, NULL);
    for (i = 0; i < 1000; ++i){
        ++mivar;
    }
    gettimeofday(&fin, NULL);

    long microseg = (fin.tv_sec - ini.tv_sec) * 1000000 + (fin.tv_usec - ini.tv_usec);

    printf("Tiempo de ejecución del bucle: %ld microsegundos.\n", microseg);

    return 0;
}
