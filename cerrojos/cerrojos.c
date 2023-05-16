#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]){
    int fd;
    struct flock cerrojo;

    // Abre el archivo
    fd = open(argv[1], O_WRONLY);

    // Configura el cerrojo
    cerrojo.l_type = F_WRLCK;   // Tipo de cerrojo: escritura
    cerrojo.l_whence = SEEK_SET;    // Origen del desplazamiento: inicio del archivo
    cerrojo.l_start = 0;    // Desplazamiento desde el origen: 0 bytes
    cerrojo.l_len = 0;  // Longitud del cerrojo: 0 bytes (hasta el final del archivo)

    // Consulta el estado del cerrojo
    if (fcntl(fd, F_GETLK, &cerrojo) == -1){
        perror("Error al consultar el estado del cerrojo.");
        exit(1);
    }
    // Si el cerrojo está disponible
    else if(cerrojo.l_type == F_UNLCK){
        // Establece el cerrojo
        if (fcntl(fd, F_SETLK, &cerrojo) == -1){
            perror("Error cerrojo no establecido.");
            exit(1);
        }

        //Apartado B: Comprobar que el cerrojo se ha establecido correctamente
        if (cerrojo.l_type != F_WRLCK){
            perror("Error al bloquear el cerrojo.");
            exit(1);
        }

        // Escribe la hora actual en el archivo
        time_t tiempo = time(NULL);
        char *horaLegible = ctime(&tiempo); // Convierte la hora a una cadena legible
        if (write(fd, horaLegible, sizeof(horaLegible)) == -1){     // Escribe la cadena en el archivo
            perror("Error al intentar escribir en el archivo.");
            exit(1);
        }
        else{
            sleep(40);
            cerrojo.l_type = F_UNLCK;

            // Libera el cerrojo
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
