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

    // Manejo de opciones de línea de comando
    while ((opt = getopt(argc, argv, "d:f:")) != -1) {
        switch (opt) {
            case 'd':
                path = optarg;
                if (lstat(path, &sb) == 0 && S_ISDIR(sb.st_mode) && (sb.st_mode & S_IRWXU) ) {
                    DIR *dir = opendir(path);

                    if (dir == NULL) {
                        perror("opendir");
                        exit(EXIT_FAILURE);
                    }

                    while ((entrada = readdir(dir)) != NULL) {
                        if (lstat(entrada->d_name, &sb) == 0) {
                            if (S_ISREG(sb.st_mode)) { // Fichero Regular
                                printf("%s (%.1f kb, %ld link%s)", entrada->d_name, (double) (sb.st_size / 1024), sb.st_nlink, sb.st_nlink == 1 ? "" : "s");
                                if (sb.st_mode & S_IXUSR || sb.st_mode & S_IXGRP || sb.st_mode & S_IXOTH) {
                                    printf("*");
                                }
                                printf("\n");
                            } else if (S_ISDIR(sb.st_mode)) { // Directorio
                                printf("[%s] (%ld link%s)\n", entrada->d_name, sb.st_nlink, sb.st_nlink == 1 ? "" : "s");
                            } else if (S_ISLNK(sb.st_mode)) { // Enlace Simbólico
                                char link_target[PATH_MAX];
                                ssize_t link_size = readlink(entrada->d_name, link_target, (double) (sb.st_size / 1024));
                                link_target[link_size] = '\0';
                                printf("%s (%s)\n", entrada->d_name, link_target);
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
                if (lstat(path, &sb) == 0 && S_ISREG(sb.st_mode)) {
                    printf("%s (inodo %ld, %.1f kb)\n", path, sb.st_ino, (double) (sb.st_size / 1024));
                } else {
                    printf("%s no es un fichero regular\n", path);
                    exit(EXIT_FAILURE);
                }
                break;
        }
    }

    return 0;
}
