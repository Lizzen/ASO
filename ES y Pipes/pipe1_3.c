// David Ferreras Díaz y Alejandro Zamorano Méndez
/*Es habitual que un proceso lea o escriba de diferentes flujos. Las llamadas select(), poll() y
epoll() permiten multiplexar diferentes las operaciones de E/S sobre múltiples flujos. Crear otra
tubería con nombre (ej. ~/tuberia2). Escribir un programa que espere con select() hasta que haya
datos listos para leer en alguna de las tuberías. El programa debe leer entonces de la tubería
correspondiente y mostrar los datos leidos en la salida estándar, indicando la tubería desde la que se
leyó. Usar lecturas no bloqueantes usando el flag O_NONBLOCK.*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>

int main() {
    int fd1, fd2, maxfd;
    fd_set readfds;
    char buf[256];
    
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
    
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(fd1, &readfds);
        FD_SET(fd2, &readfds);
        
        // Espera a que haya datos listos para leer
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1) {
            perror("Error en select()");
            exit(EXIT_FAILURE);
        }
        
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
    }
    
    // Cerrar las tuberías
    close(fd1);
    close(fd2);
    
    return 0;
}
