#include "unpipc.h"

int main(int argc, const char *argv[])
{
    int fd[2], n;
    char c;
    pid_t childpid;

    pipe(fd);
    if ((childpid = fork()) == 0) {
        sleep(3);
        if ((n = read(fd[0], &c, 1)) != 1)
            err_sys("child read error");
        printf("child read %c\n", c);
        if (write(fd[0], "c", 1) == -1)
            err_sys("child write error");

        exit(0);
    }

    write(fd[1], "p", 1);
    if ((n = read(fd[1], &c, 1)) != 1)
        err_sys("parent read error");
    printf("parent get %c\n", c);
    return 0;
}
