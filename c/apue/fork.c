#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
    printf("fork starting\n");
    pid_t pid;

    if ((pid=fork())<0)
        printf("error");
    else if (pid == 0)
        printf("child\n");
    else if (pid > 0)
        printf("parent\n");
    return 0;
}
