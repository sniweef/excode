#include "header/apue.h"
#include <stdio.h>

static void my_exit1(void)
{
    printf("my_exit1\n");
}
static void my_exit2(void)
{
    printf("my_exit2\b");
}
int main(int argc, const char *argv[])
{
    if (atexit(my_exit2) != 0)
        err_sys("can't register my_exit2");
    if (atexit(my_exit1) != 0)
        err_sys("can't register my_exit1");
    printf("main is done\n");
    //_exit(0);
    //_Exit(0);
    exit(0);
    return 0;
}
