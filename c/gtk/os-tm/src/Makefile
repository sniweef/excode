CC = gcc
FLAG = `pkg-config --cflags --libs gtk+-3.0`

all:main.o system.o process-tree.o statusbar.o
	$(CC) -Wall -o os-tm main.o system.o process-tree.o statusbar.o $(FLAG)
main.o: main.c
	$(CC) -Wall -c main.c $(FLAG)
system.o: system.c
	$(CC) -Wall -c system.c $(FLAG)
process-tree.o: process-tree.c
	$(CC) -Wall -c process-tree.c $(FLAG)
statusbar.o: statusbar.c
	$(CC) -Wall -c statusbar.c $(FLAG)
clean:
	rm -f *.o
