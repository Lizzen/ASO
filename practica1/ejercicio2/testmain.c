#include <stdio.h>
#include <stdlib.h>
#include "milibreria.h"

int main(int argc, char** argv){
    printf("Programa de test para probar las funciones de mi libreria\n");

    int res;

    res = funaux1();
    printf("- el resultado de ejecutar la funcion 1 es %d\n",res);

    res = funaux2();
    printf("- el resultado de ejecutar la funcion 2 es %d\n",res);
    

    return 0;
}