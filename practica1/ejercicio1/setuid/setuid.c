#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
	1. setuid

	Completad el código proporcionado en el fichero setuid.c para que se gestione correctamente el código de error
	devuelto por la llamada al sistema setuid siguiendo las siguientes pautas:

	• En caso de error, se mostrará por la salida de error estándar el código de error generado por la llamada, tanto en su
	versión numérica como la cadena asociada, y el programa finalizará retornando un 1.
	• En caso de éxito el programa devolverá 0.

	Usar la página de manual para comprobar el propósito de setuid, conocer su prototipo y que ficheros de cabecera
	(ficheros #include) son necesarios añadir para que no se produzcan avisos en la compilación.
	En relación con este código, responder a las siguientes cuestiones:

	• ¿Cuál es la funcionalidad de la llamada setuid?
		Se utiliza para cambiar el UID, USER ID, efectivo del proceso que llama a la función.
		
	• ¿Cómo podemos comprobar, desde la linea de comandos (el shell), el valor devuelto tras la ejecución del programa?
		Sí. Si da error, se mostrará un mensaje de "setuid error", indicando que no se ha podido cambiar el UID.
		Si por el contrario la función se realiza correctamente, el proceso mostrará por consola el nuevo UID efectivo del proceso. 
*/

int main() {
	
	if (setuid(0) == -1){
		perror("setuid error");
		exit(EXIT_FAILURE);
	}

	printf("UID efectivo: %d", geteuid());

	return 0;
} 
