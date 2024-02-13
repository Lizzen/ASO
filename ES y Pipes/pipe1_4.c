// David Ferreras Díaz
/*Completar el program del apartado 1.3 añadiendo la posibilidad de que el programa termine
cuando el usuario presione en el terminal CTR+D
Nota: El primer conjunto de descriptores de fichero que se le pasa a select es aquel sobre el que
queremos esperar hasta que haya algún dato que leer. Hay una pequeña excepción. Cuando se
alcanza el final de un fichero (o se corta la comunicación en una tubería o un socket), select se
desbloquea si el objeto está entre los especificados en su primer conjunto de descriptores de fichero.
En estos casos al hacer un read sobre uno de ellos, la llamada al sistema devolverá cero
inmediatamente, señal que nos sirve para detectar el fin del fichero o el corte de la comunicación.*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>

int main() {
    int fd1, fd2, maxfd;    // Descriptores de archivo para las tuberías
    fd_set readfds; // Conjunto de descriptores de archivo para select()
    char buf[256];  // Buffer para leer datos de las tuberías
    
    // Abrir las tuberías en modo no bloqueante
    if ((fd1 = open("~/tuberia1", O_RDONLY | O_NONBLOCK)) == -1) {
        perror("Error al abrir la tubería 1");
        exit(EXIT_FAILURE);
    }
    if ((fd2 = open("~/tuberia2", O_RDONLY | O_NONBLOCK)) == -1) {
        perror("Error al abrir la tubería 2");
        exit(EXIT_FAILURE);
    }
    
    // Calcular el descriptor máximo para la función select()
    maxfd = (fd1 > fd2) ? fd1 : fd2;
    
     // Bucle principal: espera a que haya datos disponibles en las tuberías
    while (!feof(stdin)) {
        FD_ZERO(&readfds);
        FD_SET(fd1, &readfds);
        FD_SET(fd2, &readfds);
        FD_SET(STDIN_FILENO, &readfds);
        
        // Espera a que haya datos listos para leer
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1) {
            perror("Error en select()");
            exit(EXIT_FAILURE);
        }
        
         // Leer datos de la tuberías
        if (FD_ISSET(fd1, &readfds)) {
            printf("Datos leídos de la tubería 1: ");
            ssize_t n = read(fd1, buf, sizeof(buf));
            if (n > 0) {
                write(STDOUT_FILENO, buf, n);
            }
            printf("\n");
        }
        if (FD_ISSET(fd2, &readfds)) {
            printf("Datos leídos de la tubería 2: ");
            ssize_t n = read(fd2, buf, sizeof(buf));
            if (n > 0) {
                write(STDOUT_FILENO, buf, n);
            }
            printf("\n");
        }
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            printf("Fin del programa\n");
            break;
        }
    }
    
    // Cerrar las tuberías
    close(fd1);
    close(fd2);
    
    return 0;
}
