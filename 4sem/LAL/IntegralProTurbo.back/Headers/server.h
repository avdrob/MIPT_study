#ifndef INTEGRALPROTURBO_SERVER
#define INTEGRALPROTURBO_SERVER

#include <assert.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "common.h"

#define SERV_RCV_TIMEOUT 10
#define SERV_SND_TIMEOUT 10
#define SERV_LIS_TIMEOUT 10

extern const char *usage;

void handle_error(const char*, const char*);
void fatal_err(const char*);
struct in_addr conv_addr(char*);
int conv_streamnum(char*);
int init_req_socket(void);
int init_lis_socket(struct in_addr);
void set_srvtimeo(int);
calcmsg_t form_msg(int, int);
void get_request(int, calcmsg_t);
task_t get_task(int, int*, struct sockaddr_in*, socklen_t*);
void lis_wait(int);
void send_res(int, double);

#endif
