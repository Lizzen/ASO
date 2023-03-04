#MAKEFILE

#Macros

OBJS = cerrojos.o
CFLAGS = -g -Wall

#Reglas explicitas

all: cerrojos.c
	gcc -o cerrojos cerrojos.c

cerrojos: cerrojos.o 
	gcc -o cerrojos cerrojos.o
	
cerrojos.o: cerrojos.c
	gcc $(CFLAGS) -c cerrojos.c

clean: 
	rm -f $(OBJS) cerrojos
