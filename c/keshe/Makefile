CC = gcc
all:main.o create_window.o get_user_data.o handle_data.o
	$(CC) -Wall -o WebsiteManagement main.o create_window.o get_user_data.o handle_data.o `pkg-config --libs gtk+-2.0 libgnomeui-2.0`
main.o:main.c create_window.h 
	$(CC) -Wall -c main.c `pkg-config --cflags --libs gtk+-2.0`
create_window.o:create_window.c get_user_data.h
	$(CC) -Wall -c create_window.c `pkg-config --cflags --libs gtk+-2.0 libgnomeui-2.0`
get_user_data.o:get_user_data.c create_window.h handle_data.h
	$(CC) -Wall -c get_user_data.c `pkg-config --cflags --libs gtk+-2.0 libgnomeui-2.0`
handle_data.o:handle_data.c create_window.h handle_data.h
	$(CC) -Wall -c handle_data.c `pkg-config --cflags --libs gtk+-2.0 libgnomeui-2.0`
clean:
	rm -f *.o
