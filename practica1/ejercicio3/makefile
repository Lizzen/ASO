CC=gcc
CFLAGS=-Wall -Wextra -std=c99
LDFLAGS=-lmilibreria

# Directorios
SRCDIR=src
OBJDIR=obj
BINDIR=bin
LIBDIR=lib

# Ficheros fuente y objetos
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Nombres de las librerías
STATICLIB=$(LIBDIR)/libmilibreria.a
DYNAMICLIB=$(LIBDIR)/libmilibreria.so

# Nombres de los ejecutables
APPSTATIC=$(BINDIR)/appstatic
APPDYNAMIC=$(BINDIR)/appdynamic

all: static dynamic

# Crea la librería estática
static: $(STATICLIB)

$(STATICLIB): $(OBJS)
	ar rcs $@ $^

# Crea la librería dinámica
dynamic: $(DYNAMICLIB)

$(DYNAMICLIB): $(OBJS)
	$(CC) -shared -o $@ $^

# Crea los objetos
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compila los ejecutables dtest enlazados con la librería estática y dinámica
appstatic: static
	$(CC) $(CFLAGS) -o $(APPSTATIC) $(SRCDIR)/testmain.c $(LDFLAGS)

appdynamic: dynamic
	$(CC) $(CFLAGS) -o $(APPDYNAMIC) $(SRCDIR)/testmain.c -L$(LIBDIR) -lmilibreria

clean:
	rm -rf $(OBJDIR)/*.o $(LIBDIR)/*.a $(LIBDIR)/*.so $(BINDIR)/*

.PHONY: all static dynamic appstatic appdynamic clean