CFLAGS = -Wall -g
OBJS   = uname.o 

all: uname


setuid : $(OBJS)
	gcc $(CFLAGS) -o uname $(OBJS)

clean:
	-rm *.o uname