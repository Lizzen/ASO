#MAKEFILE

#Macros

OBJS = ej2.o
CFLAGS = -g -Wall

#Reglas explicitas

all: ej2.o 
	gcc -o ej2 ej2.o
	
ej2.o: ej2.c
	gcc $(CFLAGS) -c ej2.c

clean: 
	rm -f $(OBJS) ej2

