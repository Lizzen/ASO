#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <getopt.h>
#include <unistd.h>
#include <libgen.h>

void recorre_entradas(char *path, struct stat st){
    //Abrimos el directorio y lo recorremos
    DIR *dir = opendir(path);
    if (dir == NULL){
        perror("opendir: No es un directorio.");
        exit(EXIT_FAILURE);
    }

    //Lee cada entrada
    struct dirent *entrada;
    while ((entrada = readdir(dir)) != NULL){
        //Abre cada entrada
        if (lstat(entrada->d_name, &st) == 0){
            //Si es un fichero regular
            if (S_ISREG(st.st_mode)){
                printf("%s (%lf kb, %ld link)", entrada->d_name, (double) (st.st_size / 1024), st.st_nlink);
                if (st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH) {
                    printf("*");
                }             
                printf(")\n");
            }
            //Si es un directorio
            else if (S_ISDIR(st.st_mode)){
                printf("[%s] (%ld link)\n", entrada->d_name, st.st_nlink);
            }
            //Si es un enlace simbólico
            else if (S_ISLNK(st.st_mode)){
                char *buffer = malloc(st.st_size + 1);
                if (buffer == NULL){
                    perror("malloc: error al reservar memoria para el buffer");
                    exit(EXIT_FAILURE);
                }

                ssize_t linkname = readlink(entrada->d_name, buffer, st.st_size);
                if (linkname == -1){
                    perror("readlink: error al leer a donde apunta el enlace simbólico.");
                    exit(EXIT_FAILURE);
                }

                printf("%s (%ld)\n", entrada->d_name, linkname);
            }
        }
    }
}

int main (int argc, char *argv[]){
    int option;
    struct stat st;
    char *path = optarg;

    while ((option = getopt(argc, argv, "d:f:")) != -1){
        switch(option){
            case 'd':
                //Comprobamos que es un directorio lo que se ha pasado y cumple las condiciones

                //Verifica que el "directorio" existe
                if (lstat(path, &st) == -1){
                    perror("lstat: El directorio no existe.");
                    exit(EXIT_FAILURE);
                }

                //Verifica que es eun directorio
                if (!S_ISDIR(st.st_mode)){
                    printf("S_IDIR: %s no es un directorio", path);
                }

                //Verifica que posee permisos de lectura y ejecución
                if (access(path, R_OK | X_OK) == -1){
                    perror("access: El directorio no posee permisos de lectura y ejecución");
                }
                
                recorre_entradas(path, st);
            break;
            case 'f':
                char *filename = basename(path);
                if (lstat(path, &st) == 0 && S_ISREG(st.st_mode)){
                    printf("%s (inodo %ld, %lf kb)\n", filename, st.st_ino, (double) (st.st_size / 1024));
                }
                else{
                    printf("%s no es un fichero regular.\n", filename);
                    exit(EXIT_FAILURE);
                }
            break;
            default:
                printf("No se ha pasado ninguna opción.");
                exit(EXIT_FAILURE);
            break;
        }
    }

    return 0;
}

