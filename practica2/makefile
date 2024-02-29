#MAKEFILE

#Macros

OBJS = myls.o
CFLAGS = -g -Wall

#Reglas explicitas

myls: myls.o 
	gcc -o myls myls.o
	
myls.o: myls.c
	gcc $(CFLAGS) -c myls.c

clean: 
	rm -f $(OBJS) myls
