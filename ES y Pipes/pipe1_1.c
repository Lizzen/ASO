// David Ferreras Díaz y Alejandro Zamorano Méndez
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define PIPE_NAME "~/mituberia"

int main() {
    struct stat st;
    // Crea una tubería FIFO con permisos 0666
    if (mkfifo(PIPE_NAME, 0666) == -1) {
        perror("Error al crear la tuberia");
        exit(EXIT_FAILURE);
    }
    // Obtiene información del archivo de la tubería
    if (stat(PIPE_NAME, &st) != -1){
        // Imprime el tamaño de la tubería en bytes
        printf("Tamaño de la tubería: %lld bytes\n", (long long) st.st_size);
        // Imprime los permisos de la tubería en formato octal
        printf("Permisos de la tubería: %o\n", st.st_mode & 0777);
        // Imprime la fecha del último acceso a la tubería
        printf("Último acceso a la tubería: %s", ctime(&st.st_atime));
        // Imprime la fecha de la última modificación de la tubería
        printf("Última modificación de la tubería: %s", ctime(&st.st_mtime));
    }
    
    return 0;
}
