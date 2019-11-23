CC = gcc
CFLAGS = -Wall -m64
LFLAGS = -I/usr/include/allegro5 -L/usr/lib -lallegro

all: main.o mandelbrot.o
	$(CC) $(CFLAGS) -o mandelbrot main.o mandelbrot.o $(LFLAGS)

mandelbrot.o: mandelbrot.s
	nasm -f elf64 -o mandelbrot.o mandelbrot.s

main.o: main.c mandelbrot.h
	$(CC) $(CFLAGS) -c -o main.o main.c

clean:
	rm -f *.o
