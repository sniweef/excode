#include "header/apue.h"
#include <stdio.h>
int main(int argc, const char *argv[])
{
    char name[L_tmpnam], line[MAXLINE];
    FILE *fp;

    printf("%s\n", tmpnam(NULL));
    tmpnam(name);
    printf("%s\n", name);

    if ((fp = tmpfile()) == NULL)
        err_sys("tmpfile failed");
    fputs("one line of output\n", fp);
    rewind(fp);
    if (fgets(line, sizeof(line), fp) == NULL)
        err_sys("fgets error");
    fputs(line, stdout);
    return 0;
}
