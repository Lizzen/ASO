#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    struct stat st;
    char *fifo_path = "~/mituberia";
    int res = mkfifo(fifo_path, 0666);

    if (res == -1) {
        perror("Error al crear la tuberia");
        exit(EXIT_FAILURE);
    }

    stat(fifo_path, &st);

    printf("Tipo de fichero: %d\n", st.st_mode & S_IFMT);
    printf("Permisos: %o\n", st.st_mode & 0777);
    printf("Propietario: %d\n", st.st_uid);
    printf("Grupo: %d\n", st.st_gid);
    printf("Tamaño: %ld\n", st.st_size);
    printf("Último acceso: %ld\n", st.st_atime);
    printf("Última modificación: %ld\n", st.st_mtime);
    printf("Último cambio de estado: %ld\n", st.st_ctime);
    
    return 0;
}
