#include <fcntl.h>
#include <unistd.h>
#include "header/apue.h"
int main(int argc, const char *argv[])
{
    int fd = open("append_lseek_test", O_RDWR|O_APPEND);
    if (fd > 0)
    {
        lseek(fd, 0, SEEK_SET);
        write(fd, "test", 4);
        close(fd);
    }
    return 0;
}
