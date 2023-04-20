CFLAGS = -Wall -g
OBJS   = shell.o shell_orden.o shell_jobs.o

all: Shell


Shell : $(OBJS)
	gcc $(CFLAGS) -o Shell $(OBJS)

clean:
	-rm *.o Shell
