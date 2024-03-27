#MAKEFILE

#Macros

OBJS = 2_4.o
CFLAGS = -g -Wall

#Reglas explicitas

all: 2_4.o 
	gcc -o 2_4 2_4.o
	
2_4.o: 2_4.c
	gcc $(CFLAGS) -c 2_4.c

clean: 
	rm -f $(OBJS) 2_4

