#ifndef INTEGRALPROTURBO_COMMON
#define INTEGRALPROTURBO_COMMON

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define PORT 15000
#define MAXSTRLEN 50

typedef struct calcmsg_t{
    char respstr[MAXSTRLEN];
    struct sockaddr_in calc_addr;
    int streamnum;
} calcmsg_t;

typedef struct task_t{
    double lend;
    double rend;
    int thr_num;
    long int seg_num;
} task_t;

extern const char *reqstr;
extern const char *respstr;

#endif
