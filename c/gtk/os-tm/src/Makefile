CC = gcc
FLAG = `pkg-config --cflags --libs gtk+-3.0`
all:main.o 
	$(CC) -Wall -o os-tm main.o $(FLAG)
main.o:
	$(CC) -Wall -c main.c $(FLAG)
clean:
	rm -f *.o
