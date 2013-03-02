#include "header/apue.h"

#define PSCMD   "ps -o pid,ppid,state,tty,command"

int main(int argc, const char *argv[])
{
    pid_t pid;
    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        exit(0);
    sleep(5);
    system(PSCMD);
    return 0;
}
