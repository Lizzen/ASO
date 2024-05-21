#MAKEFILE

#Macros

OBJS = ej1.o
CFLAGS = -g -Wall

#Reglas explicitas

all: ej1.o 
	gcc -o ej1 ej1.o
	
ej1.o: ej1.c
	gcc $(CFLAGS) -c ej1.c

clean: 
	rm -f $(OBJS) ej1
