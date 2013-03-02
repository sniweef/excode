#include <stdio.h>
#include "header/apue.h"

extern char **environ;

int main(int argc, const char *argv[])
{
    char **ptr;
    for (ptr = environ; ptr != NULL; ptr++)
        printf("%s\n", *ptr);
    return 0;
}
