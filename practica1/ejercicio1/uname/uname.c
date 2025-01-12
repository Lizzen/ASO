/*El comando del sistema uname muestra información sobre diversos aspectos del sistema. Escribir un programa que
muestre por la salida estándar los detalles del sistema que reporta uname y que sea equivalente a ejecutar el comando:
uname -s -n -r -v -m
Consultar la pagina de manual de la llamada al sistema uname().
• Reportar los valores obtenidos en los puestos del laboratorio.*/

#include <sys/utsname.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    struct utsname info;

    if (uname(&info) == -1){
        perror("Uname error");
        exit(EXIT_FAILURE);
    }

    printf("SO: %s\n", info.sysname);
    printf("Nombre del nodo: %s\n", info.nodename);
    printf("Versión de Kernel: %s\n", info.release);
    printf("Versión del SO: %s\n", info.version);
    printf("Hardware: %s\n", info.machine);

    return 0;
}