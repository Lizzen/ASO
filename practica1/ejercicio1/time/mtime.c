/*La función principal para obtener la fecha actual es time(). En los sistemas operativos Unix y similares, la función
time() se utiliza para obtener el tiempo actual. Más específicamente, devuelve el tiempo transcurrido en segundos
desde el 1 de enero de 1970 a las 00:00:00 UTC (marca el inicio de la epoca Unix).
La declaración de la función time() en C es la siguiente:
#include <time.h>
time_t time(time_t *t);
• time_t es un tipo de dato que típicamente representa el tiempo en segundos.
• Si el argumento t es un puntero no nulo, time() también almacena el valor del tiempo en la ubicación apuntada
por t.
El uso básico de la función es simplemente llamar a time() con un argumento NULL, y devolverá el tiempo actual.
Por ejemplo:
#include <stdio.h>
#include <time.h>
int main() {
time_t now;
now = time(NULL);
printf("Número de segundos desde el 1 de enero de 1970: %ld\n", now);
return 0;
}
Escribir el programa mtime.c que:

• Obtenga la fecha actual usando time() y la muestre en la salida estándar usando un formato de fecha legible
utilizando la función auxiliar ctime().
• Calcule la fecha de hace diez días y la muestre en la salida estándar, también en formato legible usando ctime().

Responder a las siguientes cuestiones:
• ¿Dónde se reserva espacio para el valor de la cadena que devuelve la función ctime()?
    la cadena se almacena en memoria interna estática dentro de la función. No se necesita reservar memoria o liberarla para llamar a la función.

• ¿Es necesario liberar el puntero?
   Como la cadena está almacenada en  memoria estática interna, no debe ser liberada. La función ctime() es la que maneja la gestión de memoria interna.
*/


// includes
#include <stdio.h>
#include <time.h>
int main(){
    time_t now = time(NULL);

    char *shoy, *s;
    shoy = ctime(&now);

    // consular la fecha actual

    printf("Hoy es: %s",shoy);
 

    // calcular que fecha fue hace 10 dias

    time_t diez_dias = now - (10*24*60*60);
    s = ctime(&diez_dias);
    printf("Hace 10 dias fue: %s",s);    
    
    return 0;
}    