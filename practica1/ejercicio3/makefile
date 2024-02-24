CC = gcc
CFLAGS = -Wall -g
LDFLAGS_DYNAMIC = -ldl

OBJS = testmain.o fun1src.o fun2src.o

all: libmilibreria_dynamic testmain

libmilibreria_dynamic: fun1src.o fun2src.o
	$(CC) -shared -o libmilibreria_dynamic.so fun1src.o fun2src.o

testmain: $(OBJS) libmilibreria_dynamic
	$(CC) $(CFLAGS) -o testmain $(OBJS) $(LDFLAGS_DYNAMIC)

clean:
	-rm -f *.o *.so testmain
