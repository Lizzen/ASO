CFLAGS = -Wall -g
OBJS   = localtime.o 

all: localtime


localtime : $(OBJS)
	gcc $(CFLAGS) -o localtime $(OBJS)

clean:
	-rm *.o localtime