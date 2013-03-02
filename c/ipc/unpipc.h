#ifndef UNPIPC_H
#define UNPIPC_H

//#include "config.h"

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#include <mqueue.h>

#include <semaphore.h>

#ifndef SEM_FAILED
#define SEM_FAILED ((sem_t *)(-1))
#endif

#include <sys/mman.h>

#ifndef MAP_FAILED
#define MAP_FAILED ((void *)-1)
#endif

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

#include <sys/shm.h>
#include <sys/select.h>
#include <poll.h>
#include <stropts.h>
#include <strings.h>

#include <sys/ioctl.h>
//#include <sys/filio.h>
#include <pthread.h>
//#include <door.h>

#ifdef _PSX4_NSPACE_H_TS
#undef SUCCESS
#endif
#include <rpc/rpc.h>

#define bzero(ptr, n) memset(ptr, 0, n)

#ifndef INFTIM
#define INFTIM (-1)
#define INFTIM_UNPH
#endif

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif
#define MAX_PATH 1024
#define MAXLINE 4096
#define BUFFSIZE 8192

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define DIR_MODE (FILE_MODE | S_IXUSR | S_IXGRP | S_IROTH)
#define SVMSG_MODE (MSG_R | MSG_W | MSG_R>>3 | MSG_R>>6)
#define SVSEM_MODE (SEM_R | SEM_A | SEM_R>>3 | SEM_R>>6)
#define SVSHM_MODE (SHM_R | SHM_W | SHM_R>> 3 | SHM_R>>6)

typedef void Sigfunc(int);

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

/*struct timespec {
    time_t tv_sec;
    long tv_nsec;
};*/


#ifdef __bsdi__
    #define  va_mode_t int
#else
    #define va_mode_t mode_t
#endif

#define read_lock(fd, offset, whence, len) \
            lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len) \
            lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
            lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len) \
            lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define un_lock(fd, offset, whence, len) \
            lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

#define is_read_lockable(fd, offset, whence, len) \
            (lock_test((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define is_write_lockable(fd, offset, whence, len) \
            (lock_test((fd), F_WRLCK, (offset), (whence), (len)) == 0)

#endif /* end of include guard: UNPIPC_H */
