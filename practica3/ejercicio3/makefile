#MAKEFILE

#Macros

OBJS = comunicacion_bidireccional.o
CFLAGS = -g -Wall

#Reglas explicitas

all: comunicacion_bidireccional.o 
	gcc -o comunicacion_bidireccional
 comunicacion_bidireccional.o
	
comunicacion_bidireccional.o: comunicacion_bidireccional.c
	gcc $(CFLAGS) -c comunicacion_bidireccional.c

clean: 
	rm -f $(OBJS) comunicacion_bidireccional

