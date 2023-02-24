#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;
    char *path;
    struct stat sb;
    struct dirent *entrada;

    while ((opt = getopt(argc, argv, "d:f:")) != -1) {
        switch (opt) {
            case 'd':
                path = optarg;

                // Abre el fichero si cumle las condiciones
                if (lstat(path, &sb) == 0 && S_ISDIR(sb.st_mode) && (sb.st_mode & S_IRWXU) ) { 
                    DIR *dir = opendir(path);    

                    if (dir == NULL) {
                        perror("opendir");
                        exit(EXIT_FAILURE);
                    }

                     // Lee cada entrada del directorio
                    while ((entrada = readdir(dir)) != NULL) {       
                            // Abre cada entrada
                            if (lstat(entrada->d_name, &sb) == 0) {
                                   if (S_ISREG(sb.st_mode)) { // Fichero Regular
                                        printf("%s (%.1f kb, %ld link)", entrada->d_name, (double) (sb.st_size / 1024), sb.st_nlink);
                                        if (sb.st_mode & S_IXUSR || sb.st_mode & S_IXGRP || sb.st_mode & S_IXOTH) {
                                            printf("*");
                                        }
                                        printf("\n");
                                   } else if (S_ISDIR(sb.st_mode)) { // Directorio
                                        printf("[%s] (%ld link)\n", entrada->d_name, sb.st_nlink);
                                   } else if (S_ISLNK(sb.st_mode)) { // Enlace Simbólico
                                        char *buffer = malloc(sb.st_size + 1);
                                        if (buffer == NULL){
                                            perror("malloc");
                                            exit(EXIT_FAILURE);
                                        }
                                        ssize_t linkdat = readlink(entrada->d_name, buffer, sb.st_size);
                                        if (linkdat == -1){
                                            perror("readlink");
                                            exit(EXIT_FAILURE);
                                        }
                                        printf("%s (%ld)\n", entrada->d_name, linkdat);
                                   }
                            }
                     }
                    closedir(dir);
                } else {
                    printf("El archivo no es ejecutable\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'f':
                path = optarg;
                // Abre el fichero regular
                if (lstat(path, &sb) == 0 && S_ISREG(sb.st_mode)) { 
                    printf("%s (inodo %ld, %.1f kb)\n", path, sb.st_ino, (double) (sb.st_size / 1024));
                } else {
                    printf("No es un fichero regular\n");
                    exit(EXIT_FAILURE);
                }
                break;
        }
    }

    return 0;
}
