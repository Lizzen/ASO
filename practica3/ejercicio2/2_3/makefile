#MAKEFILE

#Macros

OBJS = 2_3.o
CFLAGS = -g -Wall

#Reglas explicitas

all: 2_3.o 
	gcc -o 2_3 2_3.o
	
2_3.o: 2_3.c
	gcc $(CFLAGS) -c 2_3.c

clean: 
	rm -f $(OBJS) 2_3

