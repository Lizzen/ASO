CFLAGS = -Wall -g
OBJS   = localtime.o 

all: localtime


setuid : $(OBJS)
	gcc $(CFLAGS) -o localtime $(OBJS)

clean:
	-rm *.o localtime