#MAKEFILE

#Macros

OBJS = 2_2.o
CFLAGS = -g -Wall

#Reglas explicitas

all: 2_2.o 
	gcc -o 2_2 2_2.o
	
2_2.o: 2_2.c
	gcc $(CFLAGS) -c 2_2.c

clean: 
	rm -f $(OBJS) 2_2
