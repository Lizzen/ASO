CFLAGS = -Wall -g
OBJS   = uids.o 

all: uids


setuid : $(OBJS)
	gcc $(CFLAGS) -o uids $(OBJS)

clean:
	-rm *.o uids