CFLAGS = -Wall -g
OBJS   = sysconf.o 

all: sysconf


sysconf : $(OBJS)
	gcc $(CFLAGS) -o sysconf $(OBJS)

clean:
	-rm *.o sysconf