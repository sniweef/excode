CC = gcc
FLAG = `pkg-config --cflags --libs gtk+-3.0`
all_o = main.o system.o process-tree.o statusbar.o monitor.c

all: $(all_o)
	$(CC) -Wall -o os-tm $(all_o) $(FLAG)
main.o: main.c
	$(CC) -Wall -c main.c $(FLAG)
system.o: system.c
	$(CC) -Wall -c system.c $(FLAG)
process-tree.o: process-tree.c
	$(CC) -Wall -c process-tree.c $(FLAG)
statusbar.o: statusbar.c
	$(CC) -Wall -c statusbar.c $(FLAG)
monitor.o: monitor.c
	$(CC) -Wall -c monitor.c $(FLAG)

clean:
	rm -f *.o
