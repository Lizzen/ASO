//David Ferreras Díaz

//EJERCICIO 1

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main()
{
/* Comprobar la ocurrencia de error y notificarlo con la llamada adecuada */
    if (setuid(0) == -1){
        perror("Error del usuario efectivo");
        exit(EXIT_FAILURE);
    }
    return 1;
}

//EJERCICIO 2

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main()
{
/* Comprobar la ocurrencia de error y notificarlo con la llamada adecuada */
    if (setuid(0) == -1){
        fprintf(stderr, "Error del usuario efectivo: %d %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return 1;
}

//EJERCICIO 4

#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    struct utsname info;
    
    int ret = uname(&info);
    if (ret != 0){
        perror("Uname");
        exit(EXIT_FAILURE);
    }

    printf("SO: %s\n", info.sysname);
    printf("Node name: %s\n", info.nodename);
    printf("Kernel version: %s\n", info.release);
    printf("System version: %s\n", info.version);
    printf("Architecture: %s\n", info.machine);

    return 1;
}

//EJERCICIO 5

#include <unistd.h>
#include <stdio.h>

int main(){

    printf("Número de ticks por segundo: %ld\n", sysconf(_SC_CLK_TCK));
    printf("Nº máximo de procesos hijos: %ld\n", sysconf(_SC_CHILD_MAX));
    printf("Nº máximo de ficheros: %ld\n", sysconf(_SC_OPEN_MAX));

    return 0;
}

//EJERCICIO 6

#include <stdio.h>
#include <unistd.h>

int main(){

    printf("Número máximo de procesos simultáneos que puede ejecutar un usuario: %ld\n", sysconf(_SC_CHILD_MAX));
    printf("Tamaño de las páginas de memoria: %ld\n", sysconf(_SC_PAGE_SIZE));
    printf("Longitud máxima de los argumentos a un programa: %ld\n", sysconf(_SC_ARG_MAX));
    printf("Número máximo de ficheros que puede abrir un proceso: %ld\n", sysconf(_SC_OPEN_MAX));
    printf("Número máximo de enlaces de un fichero: %ld\n", pathconf("/", _PC_LINK_MAX));
    printf("Tamaño máximo de una ruta: %ld\n", pathconf("/", _PC_PATH_MAX));
    printf("Tamaño máximo de un nombre de fichero: %ld\n", pathconf("/", _PC_NAME_MAX));

    return 0;
}

//EJERCICIO 7

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main(){
    uid_t uid = getuid();   //uid real
    uid_t euid =  geteuid();    //uid efectivo
    struct passwd *pwd = getpwuid(uid);

    printf("UID real: %d\n", uid);
    printf("UID efectivo: %d\n", euid);

    if (uid == euid){
        printf("El ejecutable no tiene activado el bit setuid\n");
    }
    else{
        printf("El ejecutable tiene activado el bit setuid\n");
    }

    printf("Login: %s\n", pwd->pw_name);
    printf("Directorio Home: %s\n", pwd->pw_dir);
    printf("Información de usuario: %s\n", pwd->pw_gecos);

    return 0;
}

//EJERCICIO 8

#include <stdio.h>
#include <time.h>

int main(){
    time_t tiempo = time(NULL);
    char *horaLegible = ctime(&tiempo);

    printf("Hora actual: %ld\n", tiempo);
    printf("Hora legible: %s\n", horaLegible);

    return 0;
}


/*Se reserva espacio dentro de la función y se comparte con todas las llamadas, por lo tanto no es necesario liberar el puntero devuelto por la función
  ya que no se ha asignado en memoria dinámica.*/

//EJERCICIO 9

#include <stdio.h>
#include <sys/time.h>

int main(){
    int i, v;
    struct timeval ini, fin;

    gettimeofday(&ini, NULL);
    for (i = 0; i < 1000; ++i){
        ++v;
    }
    gettimeofday(&fin, NULL);

    long microseg = (fin.tv_sec - ini.tv_sec) * 1000000 + (fin.tv_usec - ini.tv_usec);

    printf("Tiempo de ejecución del bucle: %ld microseg.\n", microseg);

    return 0;
}
