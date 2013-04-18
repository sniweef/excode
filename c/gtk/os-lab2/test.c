#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("usage: %s [source] [dest].\n", argv[0]);
        return -1;
    }
    syscall(312, argv[1], argv[2]);
    return 0;
}
