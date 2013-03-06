#ifndef UNP_H
#define UNP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>

#include <sys/select.h>
#include <sys/sysctl.h>
#include <poll.h>
#include <sys/event.h>
#include <strings.h>

#include <sys/ioctl.h>
#include <sys/filio.h>
#include <sys/sockio.h>
#include <pthread.h>
#include <net/if_dl.h>
#include <netinet/sctp.h>

#ifdef __osf__
#undef recv
#undef send
#define recv(a,b,c,d) recvfrom(a,b,c,d,0,0)
#define send(a,b,c,d) sendto(a,b,c,d,0,0)
#endif

#ifndef INADDR_NONE
    #define INADDR_NONE
#endif

#ifndef SHUT_RD
#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2
#endif

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 46
#endif

struct unp_in_pktinfo {
    struct in_addr  ipi_addr;
    int             ipi_ifindex;
};

#ifndef CMSG_LEN
#define CMSG_LEN(size) (sizeof(struct cmsghdr)+(size))
#endif
#ifndef CMSG_SPACE
#define CMSG_SPACE(size) (sizeof(struct cmsghdr)+(size))
#endif

#ifndef SUN_LEN(su)
#define SUN_LEN(su) \
    (sizeof(*(su))-sizeof((su)->sun_path)+strlen((su)->sun_path))
#endif

#ifndef AF_LOCAL
#define AF_LOCAL AF_UNIX
#endif
#ifndef PF_LOCAL
#define PF_LOCAL PF_UNIX
#endif

#ifndef INFTIM
#define INFTIM (-1)
#endif

#define LISTENQ 1024
#define MAXLINE 4096
#define BUFFSIZE    8192
#define SERV_PORT   9877
#define SERV_PORT_STR   "9877"
#define UNIXSTR_PATH    "/tmp/unix.str"
#define UNIXDG_PATH     "/tmp/unix.dg"

#define SA  struct sockaddr
#define HAVE_STRUCT_SOCKADDR_STORAGE

#define FILE_MODE   (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define DIR_MODE    (FILE_MODE|S_IXUSR|S_IXGRP|S_IXOTH)

typedef void Sigfunc(int);
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#endif /* end of include guard: UNP_H */
