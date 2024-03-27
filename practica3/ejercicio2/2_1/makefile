#MAKEFILE

#Macros

OBJS = 2_1.o
CFLAGS = -g -Wall

#Reglas explicitas

all: 2_1.o 
	gcc -o 2_1 2_1.o
	
2_1.o: 2_1.c
	gcc $(CFLAGS) -c 2_1.c

clean: 
	rm -f $(OBJS) 2_1
