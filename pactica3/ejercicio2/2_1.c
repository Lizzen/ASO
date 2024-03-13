#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main (char *argv[]){
    char *pipe= "tuberia1";

    // Crea una tubería FIFO con permisos de lectura y escritura rw-rw-rw
    if (mkfifo(pipe, 0666) == -1){
        perror("mkfifo: Error al crear una tubería FIFO con permisos de lectura y escritura.\n");
        exit(EXIT_FAILURE);
    }

    struct stat st;

    // Obtiene e imprime la información de la tubería 
    if (stat(pipe, &st) != -1){
        printf("Tamaño de la tubería: %lld bytes\n", (long long) st.st_size);
        printf("Permisos de la tubería: %o\n", st.st_mode & 0777);
        printf("Último acceso a la tubería: %s", ctime(&st.st_atime));
        print("Última modificación de la tubería: %s", ctime(&st.st_mtime));
    }
    return 0;
}