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