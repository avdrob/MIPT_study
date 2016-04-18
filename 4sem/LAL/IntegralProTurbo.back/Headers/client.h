#ifndef INTEGRALPROTURBO_CLIENT
#define INTEGRALPROTURBO_CLIENT

#include <math.h>
#include <pthread.h>
#include "common.h"

#define MAXCALCSNUM 255
#define MAXRECFAILS 2
#define CLI_RCV_TIMEOUT 40
#define CLI_SND_TIMEOUT 10

typedef struct thrarg_t{
    struct sockaddr_in calc_addr;
    task_t task;
    int sockfd;
} thrarg_t;

extern const char *usage;
extern const char *BRDCAST;

void handle_error(const char*, const char*);
void fatal_err(const char*);
int conv_calcnum(char*);
double conv_segmend(char*);
long int conv_segnum(char*);
int init_brd_socket(void);
int init_con_socket(void);
void set_clitimeo(int);
calcmsg_t *detect_calcs(int, int*);
int get_streamnum(int, calcmsg_t*);
int *distr_task(double, double, long int, int, calcmsg_t*);
void send_task(int*, task_t*, struct sockaddr_in*);
double get_res(int, int*);

#endif
