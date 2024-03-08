#MAKEFILE

#Macros

OBJS = Tuberia_sin_nombre.o
CFLAGS = -g -Wall

#Reglas explicitas

all: Tuberia_sin_nombre.o 
	gcc -o Tuberia_sin_nombre Tuberia_sin_nombre.o
	
Tuberia_sin_nombre.o: Tuberia_sin_nombre.c
	gcc $(CFLAGS) -c Tuberia_sin_nombre.c

clean: 
	rm -f $(OBJS) Tuberia_sin_nombre
