#include "unpipc.h"

#define SERV_FIFO "/tmp/serv_fifo"

int main(int argc, const char *argv[])
{
    if ((mkfifo(SERV_FIFO, FILE_MODE) < 0) && (errno != EEXIST)) 
        err_sys("canot create %s" SERV_FIFO);
    open(SERV_FIFO, O_RDONLY, 0);
    open(SERV_FIFO, O_WRONLY, 0);
    return 0;
}
