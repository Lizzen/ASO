#MAKEFILE

#Macros

OBJS = señales.o
CFLAGS = -g -Wall

#Reglas explicitas

all: señales.o 
	gcc -o señales señales.o
	
señales.o: señales.c
	gcc $(CFLAGS) -c señales.c

clean: 
	rm -f $(OBJS) señales

