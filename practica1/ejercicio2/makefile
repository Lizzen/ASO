CC = gcc
CFLAGS = -Wall -g
LDFLAGS_STATIC = -L. -lmilibreria_static
LDFLAGS_DYNAMIC = -L. -lmilibreria_dynamic

OBJS = testmain.o fun1src.o fun2src.o

all: libmilibreria_static libmilibreria_dynamic testmain_static testmain_dynamic

libmilibreria_static: fun1src.o fun2src.o
	ar rcs libmilibreria_static.a fun1src.o fun2src.o

libmilibreria_dynamic: fun1src.o fun2src.o
	$(CC) -shared -o libmilibreria_dynamic.so fun1src.o fun2src.o

testmain_static: $(OBJS) libmilibreria_static
	$(CC) $(CFLAGS) -o testmain_static $(OBJS) $(LDFLAGS_STATIC)

testmain_dynamic: $(OBJS) libmilibreria_dynamic
	$(CC) $(CFLAGS) -o testmain_dynamic $(OBJS) $(LDFLAGS_DYNAMIC)

clean:
	-rm -f *.o *.a *.so testmain_static testmain_dynamic
