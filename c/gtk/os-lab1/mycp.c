#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define FILE_MODE   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("usage: %s [source] [dest]\n", argv[0]);
        return -1;
    }
    int source, dest;
    char buffer[BUFFER_SIZE];

    if ((source = open(argv[1], O_RDONLY)) < 0) {
        printf("open %s failed.\n", argv[1]);
        return -1;
    }
    if ((dest = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, FILE_MODE)) < 0) {
        printf("open or create %s failed.\n", argv[1]);
        return -1;
    }

    int n;
    while ((n = read(source, buffer, BUFFER_SIZE)) > 0) {
        write(dest, buffer, n);
    }

    close(source);
    close(dest);
    return 0;
}
