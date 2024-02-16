CFLAGS = -Wall -g
OBJS   = sysconf.o 

all: sysconf


setuid : $(OBJS)
	gcc $(CFLAGS) -o sysconf $(OBJS)

clean:
	-rm *.o sysconf