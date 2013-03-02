#include <stdio.h>
#include "header/apue.h"

int main(int argc, const char *argv[])
{
    pid_t pid;

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0) {
        if (execl("/home/sniweef/programming/c/apue/echo_all", 
                  "echo_all", (char *)0) < 0)
            err_sys("execl error");
    } else {
        wait(NULL);
    }
        
    return 0;
}
