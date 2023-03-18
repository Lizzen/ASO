#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define PIPE_NAME "~/mituberia"

int main() {
    struct stat st;

    if (mkfifo(PIPE_NAME, 0666) == -1) {
        perror("Error al crear la tuberia");
        exit(EXIT_FAILURE);
    }

    if (stat(PIPE_NAME, &st) != -1){
        printf("Tamaño de la tubería: %lld bytes\n", (long long) st.st_size);
        printf("Permisos de la tubería: %o\n", st.st_mode & 0777);
        printf("Último acceso a la tubería: %s", ctime(&st.st_atime));
        printf("Última modificación de la tubería: %s", ctime(&st.st_mtime));
    }
    
    return 0;
}
