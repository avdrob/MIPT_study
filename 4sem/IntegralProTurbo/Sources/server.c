#include "../Headers/server.h"

const char *reqstr="I do need some calculation";
const char *respstr="Yes, my captain!";
const char *usage="Wrong arguments\nserver [connection address] [streams number]\n";

struct in_addr conv_addr(char *addrstr)
{
    struct in_addr addr;
    if (inet_pton(AF_INET, (const char*)addrstr, (void*)&addr)==0)
        fatal_err("Wrong address format");
    
    return addr;
}

int conv_streamnum(char *streamstr)
{
    char *endptr;
    int n=(int)strtol(streamstr, (char**)&endptr, 10);
    if (!((endptr[0]==' ') | (endptr[0]=='\n') | (endptr[0]=='\0')) || !(n>0))
        fatal_err("The second arg is wrong\nStreams number must be positive integer");
        
    return n;
}
int init_req_socket()
{
    fprintf(stdout, "Initializing socket for receiving a broadcast request... ");
    fflush(stdout);
    
    int reuse=1;
    int sockfd;
    
    if ((sockfd=socket(AF_INET, SOCK_DGRAM, 0))<0)
        handle_error("\nCannot create request socket.",  "Terminated");
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))<0)
        handle_error("\nCannot enable SO_REUSEADDR option.", "Terminated");
        
    struct sockaddr_in servaddr={.sin_family=AF_INET, .sin_port=htons(PORT), .sin_addr.s_addr=htonl(INADDR_ANY)};
    if (bind(sockfd, (struct sockaddr*)&servaddr, (socklen_t)sizeof(servaddr))<0)
        handle_error("\nCannot bind server request address.", "Terminated");
    
    fprintf(stdout, "Completed\n");
    fflush(stdout);
    
    return sockfd;
}

int init_lis_socket(struct in_addr locaddr)
{
    fprintf(stdout, "Initializing listening socket... ");
    fflush(stdout);
    
    int lissockfd;
    if ((lissockfd=socket(AF_INET, SOCK_STREAM, 0))<0)
        handle_error("\nCannot create connection socket.", "Terminated");
    
    struct sockaddr_in servaddr={.sin_family=AF_INET, .sin_port=htons(0), .sin_addr=locaddr};
    if (bind(lissockfd, (struct sockaddr*)&servaddr, (socklen_t)sizeof(servaddr))<0)
        handle_error("\nCannot bind server connection address.", "Terminated");
        
    if (listen(lissockfd, 1)<0)
        handle_error("\nCannot mark connnection socket as passive.", "Terminated");
    
    int flags=fcntl(lissockfd, F_GETFL);
    if ((flags<0) || (fcntl(lissockfd, F_SETFL, flags|O_NONBLOCK)<0))
        handle_error("\nCannot set flags on listen socket.", "Terminated");
    
    fprintf(stdout, "Completed\n");
    fflush(stdout);
    
    return lissockfd;
}

calcmsg_t form_msg(int consockfd, int streamnum)
{
    fprintf(stdout, "Creating info message for client... ");
    fflush(stdout);
    
    calcmsg_t msg;
    
    struct sockaddr_in addr;
    socklen_t addrlen=sizeof(addr);
    if (getsockname(consockfd, (struct sockaddr*)&addr, &addrlen)<0)
        handle_error("\nCannot get socket name.", "Terminated");
    msg.calc_addr=addr;
    
    strcpy((char*)msg.respstr, respstr);
    assert(strcmp((char*)msg.respstr, respstr)==0);
    
    msg.streamnum=streamnum;
    
    fprintf(stdout, "Completed\n");
    fflush(stdout);
    
    return msg;
}
    
void get_request(int reqsockfd, calcmsg_t msg)
{
    fprintf(stdout, "Waiting for broadcast request... ");
    fflush(stdout);
    
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    char recvstr[MAXSTRLEN];
    while (1){
        if ((recvfrom(reqsockfd, (void*)recvstr, strlen(reqstr)+1, MSG_WAITALL, (struct sockaddr*)&cliaddr, &clilen)==
            strlen(reqstr)+1) && (strcmp((char*)recvstr, reqstr)==0))
            break;
    }
    
    if (sendto(reqsockfd, (void*)&msg, sizeof(msg), 0, (struct sockaddr*)&cliaddr, clilen)<0)
        handle_error("\nCannot send response message.", "Restarting...\n");
    
    fprintf(stdout, "Request is got and answered\n");
    fflush(stdout);
    
    return;
}

task_t get_task(int lissockfd, int *consockfd, struct sockaddr_in *cliaddr, socklen_t *clilen)
{
    fprintf(stdout, "Getting a task... ");
    fflush(stdout);
    
    lis_wait(lissockfd);
    if ((*consockfd=accept(lissockfd, (struct sockaddr*)cliaddr, clilen))<0)
        handle_error("\nCannot accept connection request.", "Restarting...\n");
    
    set_srvtimeo(*consockfd);
        
    task_t task;
    if (read(*consockfd, (void*)&task, sizeof(task))!=sizeof(task))
        handle_error("\nProblems while reading from connection socket. Connection is broken.", "Restarting...\n");
    
    fprintf(stdout, "Completed\n");
    fflush(stdout);
    
    return task;
}

void lis_wait(int sockfd)
{
    fd_set rfds;
    int retval;
    struct timeval timeout={.tv_sec=SERV_LIS_TIMEOUT, .tv_usec=0};
    
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    
    retval=select(sockfd+1, &rfds, NULL, NULL, &timeout);
    
    if (retval==0)
        fatal_err("\nCannot accept connection request: timeout is over. Restarting...\n");
    if (retval<0)
        handle_error("\nCannot accept connetcion request.", "Restarting...\n");
    
    return;
}

void send_res(int consockfd, double res)
{   
    fprintf(stdout, "Sending result to the client...");
    fflush(stdout);
    
    if (write(consockfd, (void*)&res, sizeof(res))!=sizeof(res))
        handle_error("\nProblems while sending the result. Connection is broken", "Restarting...\n");
    
    fprintf(stdout, "Completed\n\n");
    fflush(stdout);
    
    return;
}

void set_srvtimeo(int sockfd)
{
    struct timeval timeout={.tv_sec=SERV_RCV_TIMEOUT, .tv_usec=0};
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void*)&timeout, sizeof(timeout))<0)
        handle_error("\nCannot set receive timeout", "Restarting...\n");
    timeout.tv_sec=SERV_SND_TIMEOUT;
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (void*)&timeout, sizeof(timeout))<0)
        handle_error("\nCannot set send timeout", "Restarting...\n");
        
    return;
}

void handle_error(const char *err_msg, const char *info_msg)
{
    perror(err_msg);
    if (info_msg!=NULL){
        fprintf(stderr, "%s\n", info_msg);
        fflush(stderr);
    }
    
    exit(EXIT_FAILURE);
}

void fatal_err(const char *err_msg)
{
    fprintf(stderr, "%s\n", err_msg);
    fflush(stderr);
    exit(EXIT_FAILURE);
}

double func(double x){
	return 4*x*x*x;
}
