#include "header/apue.h"

void f1()
{
    printf("f1() begin\n");
    if (fork() < 0)
        err_sys("fork error");
    printf("f1() end\n");
}
void f2()
{
    int buf[1000];
    int i;
    printf("f2() begin\n");
    for (i = 0; i < 1000; i++)
        buf[i] = 0;
    printf("f2() end\n");
}
int main(int argc, const char *argv[])
{
    f1();
    f2();
    return 0;
}
