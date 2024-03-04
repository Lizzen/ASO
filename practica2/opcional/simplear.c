// David Ferreras Díaz y Alejandro Zamorano Méndez 

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Estructura proporcionada en el enunciado
#define ARMGICO "<arspl>\n" /* Numero Magico simplear */
#define MAGSIZE 8 /* Longitud Numero Magico */
struct ar_ghdr /* Cabecera global */
{
char ar_magic[MAGSIZE]; /* Numero Magico fichero simplear */
int nfiles; /* Numero de miembros */
};

struct ar_ghdr { /* Cabecera global */
char ar_magic[MAGSIZE]; /* Numero Magico fichero simplear */
char ar_fstmoff[12]; /* Desplazamiento al primer miembro */
char ar_lstmoff[12]; /* Desplazamiento al ultimo miembro */
char ar_freeoff[12]; /* Desplazamiento a la lista de miembros libres */
char fl_memoff[12]; /* Desplazamiento a la tabla de miembros */
char fl_gstoff[12]; /* Desplazamiento a la tabla de símbolos (librerias) */
} ;

struct ar_lhdr /* Cabecera de miembro de archivo */
{
char ar_memname[16]; /* Nombre del miembro (maximo de 15 caracteres) */
long ar_size; /* Tamaño del contenido del miembro (en bytes) */
char ar_date[12]; /* Fecha */
int ar_uid; /* ID de usuario */
int ar_gid; /* ID de grupo */
char ar_mode[12]; /* Modo - octal */
};

//Función -c (crea el archivo)
void create_archivo(char *argv[]){
    struct ar_ghdr stAr;    /*Struct ar de Cabecera Global*/

    int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1){
        perror("open: Error al crear el archivo.");
        exit(EXIT_FAILURE);
    }
}

//Función  -r(Inserta archivos en un fichero)
void insert_archivo(){}

//Función -x (Extrae el fichero)
void extract_archivo(){}

int main(int argc, char *argv[]){
    int option;

    while ((option = getopt(argc,argv , "c:r:x:"))){
        switch(option){
            //Creación de un archivo
            case 'c':
                create_archivo(argv);
            break;

            //Inserción de un fichero en el archivo
            case 'r':
                insert_archivo();
            break;

            //Extracción de un fichero
            case 'x':
                extract_archivo();
            break;

            default: 
                printf("No se ha especificado ninguna de las opciones (-c/-r/-x)\n");
                exit(EXIT_FAILURE);
            break;
        }
    }
    return 0;
}