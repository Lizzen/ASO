#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "milibreria.h"

int main(int argc, char** argv){
    printf("Programa de test para probar las funciones de mi libreria\n");

    // Limpia errores previos
    dlerror();

    void *carga = dlopen("./libmilibreria.so", RTLD_LAZY);
    if (!carga){
        fprintf(stderr, "Error al cargar las librerías dinámicas.");
        exit(EXIT_FAILURE);
    }

    //Busca las funciones en sus correspondientes librerías
    int (*funaux1)();
    int (*funaux2)();
    funaux1 = dlsym(carga, "funaux1");
    funaux1 = dlsym(carga, "funaux2");

    // Verifica si hay errores al intentar buscar las funciones en las librerías
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        fprintf(stderr, "Error al buscar las funciones en la libreria dinámica: %s\n", dlsym_error);
        dlclose(carga);
        exit(EXIT_FAILURE);
    }

    int res;

    res = funaux1();
    printf("- el resultado de ejecutar la funcion 1 es %d\n",res);

    res = funaux2();
    printf("- el resultado de ejecutar la funcion 2 es %d\n",res);
    
    dlclose(carga);

    return 0;
}