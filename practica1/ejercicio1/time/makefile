CFLAGS = -Wall -g
OBJS   = time.o 

all: time


setuid : $(OBJS)
	gcc $(CFLAGS) -o time $(OBJS)

clean:
	-rm *.o time