CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0`
LFLAGS=`pkg-config --libs gtk+-3.0`
SRC=./proyecto.c
EXE=exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LFLAGS)

correr:
	./exe

limpiar:
	rm exe
