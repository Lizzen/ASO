/*Las funciones sysconf() y pathconf() permiten consultar información sobre la configuración del sistema y el
sistema de ficheros respectivamente. Escribir un programa que muestre los siguientes parámetros y reportar los valores
obtenidos en los puestos del laboratorio.*/

#include <stdio.h>
#include <unistd.h>

int main(){
    printf("Ticks por segundo: %ld\n", sysconf(_SC_CLK_TCK));
    printf("Número máximo de procesos simultáneos que puede ejecutar un usuario: %ld\n", sysconf(_SC_CHILD_MAX));
    printf("Número máximo de ficheros que puede abrir un proceso: %ld\n", sysconf(_SC_OPEN_MAX));
    printf("Tamaño de las páginas de memoria: %ld\n", sysconf(_SC_PAGESIZE));
    printf("Longitud máxima de los argumentos a un programa: %ld\n", sysconf(_SC_ARG_MAX));
    printf("Número máximo de enlaces de un fichero: %ld\n", pathconf("/",_PC_LINK_MAX));
    printf("Tamaño máximo de una ruta en el sistema de ficheros %ld\n", pathconf("/",_PC_PATH_MAX));
    printf("Tamaño máximo de nombre de fichero en el sistema de ficheros %ld\n", pathconf("/",_PC_NAME_MAX));

    return 0;
}