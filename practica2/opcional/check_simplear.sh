#!/bin/bash

# Comprobar si el programa simplear está en el directorio actual y es ejecutable
if [[ ! -x ./simplear ]]; then
    echo "simplear no encontrado o no es ejecutable" >&2
    exit 1
fi

# Comprobar si existe el directorio tmp y eliminarlo si existe
if [[ -d tmp ]]; then
    rm -rf tmp
fi

# Crear un nuevo directorio temporal tmp
mkdir tmp
cd tmp || exit 1

# Crear los ficheros file1.txt, file2.txt, file3.dat
echo "Hello world!" > file1.txt
head -n 10 /etc/passwd > file2.txt
head -c 1024 /dev/urandom > file3.dat

# Invocar simplear para crear el archivo miarchivoar.ar
../simplear -cr miarchivoar.ar file1.txt file2.txt file3.dat

# Crear el directorio out y copiar el archivo miarchivoar.ar al nuevo directorio
mkdir out
cp miarchivoar.ar out/
cd out || exit 1

# Extraer el contenido del archivo
../simplear -x miarchivoar.ar

# Comprobar que los ficheros extraídos son idénticos a los originales
diff -q file1.txt ../file1.txt
if [[ $? -ne 0 ]]; then
    echo "Error: file1.txt difiere del original" >&2
    exit 1
fi

diff -q file2.txt ../file2.txt
if [[ $? -ne 0 ]]; then
    echo "Error: file2.txt difiere del original" >&2
    exit 1
fi

diff -q file3.dat ../file3.dat
if [[ $? -ne 0 ]]; then
    echo "Error: file3.dat difiere del original" >&2
    exit 1
fi

# Si todo es correcto
cd ../..
echo "Correcto"
exit 0
