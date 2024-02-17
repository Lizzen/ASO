CFLAGS = -Wall -g
OBJS   = gettime.o 

all: gettime


setuid : $(OBJS)
	gcc $(CFLAGS) -o gettime $(OBJS)

clean:
	-rm *.o gettime
