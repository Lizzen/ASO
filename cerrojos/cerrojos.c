#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]){
    int fd;
    struct flock cerrojo;

    fd = open(argv[1], O_WRONLY);

    cerrojo.l_type = F_WRLCK;
    cerrojo.l_whence = SEEK_SET;
    cerrojo.l_start = 0;
    cerrojo.l_len = 0;

    

    if (fcntl(fd, F_GETLK, &cerrojo) == -1){
        perror("Error al consultar el estado del cerrojo.");
        exit(1);
    }
    else if(cerrojo.l_type == F_UNLCK){
        if (fcntl(fd, F_SETLK, &cerrojo) == -1){
            perror("Error cerrojo no establecido.");
            exit(1);
        }

        //Apartado B
        if (cerrojo.l_type != F_WRLCK){
            perror("Error al bloquear el cerrojo.");
            exit(1);
        }

        time_t tiempo = time(NULL);
        char *horaLegible = ctime(&tiempo);
        if (write(fd, horaLegible, sizeof(horaLegible)) == -1){
            perror("Error al intentar escribir en el archivo.");
            exit(1);
        }
        else{
            sleep(40);
            cerrojo.l_type = F_UNLCK;

            if (fcntl(fd, F_SETLK, &cerrojo) == -1){
                perror("Error cerrojo no liberado.");
                exit(1);
            }
        }
    }
    else {
        printf("El cerrojo está bloqueado\n");
        exit(1);
    }

    if (close(fd) == -1){
        perror("Error al intentar cerrar el archivo.");
        exit(1);
    }

    return 0;
}
