#include "unpipc.h"

int main(int argc, const char *argv[])
{
    int fd;
    fd = open("test1.data", O_RDWR | O_CREAT, FILE_MODE);

    read_lock(fd, 0, SEEK_SET, 0);
    printf("%s: parent has read lock\n", gf_time());

    if (fork() == 0) {
        sleep(1);
        printf("%s: first child tries to obtain write lock\n", gf_time());
        writew_lock(fd, 0, SEEK_SET, 0);
        printf("%s: first child obtains write lock\n", gf_time());
        sleep(2);
        un_lock(fd, 0, SEEK_SET, 0);
        printf("%s: first child releases write lock\n", gf_time());
        exit(0);
    }
    if (fork() == 0) {
        sleep(3);
        printf("%s: second child tries to obtain read lock\n", gf_time());
        readw_lock(fd, 0, SEEK_SET, 0);
        printf("%s: second child obtains read lock\n", gf_time());
        sleep(4);
        un_lock(fd, 0, SEEK_SET, 0);
        printf("%s: second child releases read lock\n", gf_time());
        exit(0);
    }
    sleep(5);
    un_lock(fd, 0, SEEK_SET, 0);
    printf("%s: parent releases read lock\n", gf_time());
    return 0;
}
