/*Escribir un programa uids.c que muestre:
• El uid efectivo y el uid real del usuario que ejecutó el programa, e indique si el correspondiente ejecutable tiene
activado el bit setuid.
• Adicionalmente, utilizando la llamada getpwuid() mostrar el nombre de usuario del usuario real, su directorio
home y el shell que utiliza.*/

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    uid_t uid = getuid();
    uid_t euid = geteuid();
    struct passwd *pwd = getpwuid(uid);
    
    printf("UID efectivo del programa: %d\n", euid);
    printf("UID real del programa: %d\n", uid);

    if (uid == euid){ printf("El ejecutable tiene activado el bit setuid.\n");}
    else{printf("El ejecutable tiene desactivado el bit setuid.\n");}

    printf("Nombre de usuario: %s\n", pwd->pw_name);
    printf("Directorio home: %s\n", pwd->pw_dir);
    printf("Shell: %s\n", pwd->pw_shell);


    return 0;
}