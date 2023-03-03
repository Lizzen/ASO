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

    char *buffer = malloc(500);
    time_t tiempo = time(NULL);
    char *horaLegible = ctime(&tiempo);
    buffer = horaLegible;

    if (fcntl(fd, F_SETLK, &cerrojo) != -1){
        if (write(fd, &buffer, sizeof(buffer)) == -1){
            close(fd);
            exit(0);
        }
        else{
            cerrojo.l_type = F_WRLCK;
            sleep(40);
            cerrojo.l_type = F_UNLCK;
        }
    }

    return 0;
}
