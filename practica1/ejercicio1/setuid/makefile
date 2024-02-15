CFLAGS = -Wall -g
OBJS   = setuid.o 

all: setuid


setuid : $(OBJS)
	gcc $(CFLAGS) -o setuid $(OBJS)

clean:
	-rm *.o setuid