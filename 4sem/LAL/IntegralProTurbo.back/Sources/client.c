#include "../Headers/client.h"

const char *usage="Wrong arguments\nclient [a] [b] [segnum] [min calcs number]\n";
const char *reqstr="I do need some calculation";
const char *respstr="Yes, my captain!";
const char *BRDCAST="127.255.255.255";

int init_brd_socket()
{
    fprintf(stdout, "Initializing socket for sending a broadcast request... ");
    fflush(stdout);
    
    int brdcast=1;
    int sockfd;
    
    if ((sockfd=socket(AF_INET, SOCK_DGRAM, 0))<0)
        handle_error("\nCannot create broadcast socket.", "Terminated");
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &brdcast, sizeof(brdcast))<0)
        handle_error("\nCannot enable SO_BROADCAST option.", "Terminated");
    
    set_clitimeo(sockfd);
    
    fprintf(stdout, "Completed\n");
    fflush(stdout);
        
    return sockfd;
}

int init_con_socket()
{
    int consockfd;
    if ((consockfd=socket(AF_INET, SOCK_STREAM, 0))<0)
        handle_error("\nCannot create connection socket.", "Terminating");
    
    struct sockaddr_in cliaddr={.sin_family=AF_INET, .sin_port=htons(0), .sin_addr.s_addr=htonl(INADDR_ANY)};
    if (bind(consockfd, (struct sockaddr*)&cliaddr, (socklen_t)sizeof(cliaddr))<0)
        handle_error("\nCannot bind client connection address.", "Terminating");
        
    set_clitimeo(consockfd);
        
    return consockfd;
}

int conv_calcnum(char *str)
{
    char *endptr;
    int n=(int)strtol(str, (char**)&endptr, 10);
    if (!((endptr[0]==' ') | (endptr[0]=='\n') | (endptr[0]=='\0')) || !(n>0))
        fatal_err("The forth arg is wrong\nMin number of calculators must be positive integer");
        
    return n;
}

double conv_segmend(char *str)
{
    char *errstr;
	double d=strtod(str, (char**)&errstr);
    if (!((errstr[0]==' ') | (errstr[0]=='\n') | (errstr[0]=='\0')))
        fatal_err("The first/second arg is wrong\nSegment end must be decimal number");
        
    return d;
}

long int conv_segnum(char *str)
{
    char *endptr;
    int n=strtol(str, (char**)&endptr, 10);
    if (!((endptr[0]==' ') | (endptr[0]=='\n') | (endptr[0]=='\0')) || !(n>0))
        fatal_err("The third arg is wrong\nNumber of segmensts to calculate must be positive integer");
        
    return n;
}

calcmsg_t *detect_calcs(int sockfd, int *calcnum)
{
    fprintf(stdout, "Detecting %d calculating servers...", *calcnum);
    fflush(stdout);
    
    calcmsg_t *calcs=(calcmsg_t*)calloc(MAXCALCSNUM, sizeof(calcmsg_t));
    if (calcs==NULL)
        fatal_err("\nCannot allocate memory for calculators detection. Terminated");
        
    struct sockaddr_in servaddr={.sin_family=AF_INET, .sin_port=htons(PORT), .sin_addr.s_addr=inet_addr(BRDCAST)};
    struct sockaddr_in cliaddr={.sin_family=AF_INET, .sin_port=htons(0), .sin_addr.s_addr=htonl(INADDR_ANY)};
    
    if (bind(sockfd, (struct sockaddr*)&cliaddr, (socklen_t)sizeof(cliaddr))<0)
        handle_error("\nCannot bind client address.", "Terminated");
    
    if (sendto(sockfd, (void*)reqstr, strlen(reqstr)+1, 0, (struct sockaddr*)&servaddr, (socklen_t)sizeof(servaddr))<0)
        handle_error("\nCannot send broadcast message.", "Terminated");
        
    int cnt=0;
    int fails=0;
    while (cnt<(*calcnum)){
        if ((recvfrom(sockfd, (void*)&(calcs[cnt]), sizeof(calcmsg_t), MSG_WAITALL, NULL, NULL)==sizeof(calcmsg_t)) &&
            (strcmp((char*)calcs[cnt].respstr, respstr)==0) && (cnt<MAXCALCSNUM)){
            fails=0;
            cnt++;
        }
        else {
            fails++;
            if (fails==MAXRECFAILS)
                handle_error("\nNot enough calculation servers in the network.", "Terminated");
            if (sendto(sockfd, (void*)reqstr, strlen(reqstr)+1, 0, (struct sockaddr*)&servaddr, (socklen_t)sizeof(servaddr))<0)
                handle_error("\nCannot send broadcast message", "Terminated");
        }
    }
    
    /*if ((recvfrom(sockfd, (void*)&(calcs[cnt]), sizeof(calcmsg_t), MSG_DONTWAIT, NULL, NULL)==sizeof(calcmsg_t)) && 
        (strcmp((char*)calcs[cnt].respstr, respstr)==0) && (cnt<MAXCALCSNUM)) {
        cnt++;
        while (1){
            if ((recvfrom(sockfd, (void*)&(calcs[cnt]), sizeof(calcmsg_t), MSG_DONTWAIT, NULL, NULL)==sizeof(calcmsg_t)) &&
                (strcmp((char*)calcs[cnt].respstr, respstr)==0) && (cnt<MAXCALCSNUM))
                cnt++;
            else break;
        }
    }
    *calcnum=cnt;*/
    
    fprintf(stdout, "Completed\n");
    fflush(stdout);
    
    return calcs;
}

int get_streamnum(int calcnum, calcmsg_t *calcs)
{
    int i;
    int total_streamnum=0;
    for (i=0; i<calcnum; i++)
        total_streamnum+=calcs[i].streamnum;
    
    return total_streamnum;
}

int *distr_task(double a, double b, long int total_segnum, int calcnum, calcmsg_t *calcs)
{
    fprintf(stdout, "Distributing task among calculators...");
    fflush(stdout);
    
    task_t *tasks=(task_t*)calloc(calcnum, sizeof(task_t));
    int *confds=(int*)calloc(calcnum, sizeof(int));
    if ((tasks==NULL) || (confds==NULL))
        fatal_err("Cannot allocate memory for task distribution. Terminated");
        
    int total_streamnum=get_streamnum(calcnum, calcs);
    
    int i;
    long int cur_seg_prop_num;
    double segm=(b-a)/total_segnum;
    double prop_factor=total_segnum/total_streamnum;
    
    cur_seg_prop_num=(long int)round(prop_factor*calcs[0].streamnum);
    tasks[0]=(task_t){.lend=a, .rend=a+cur_seg_prop_num*segm, .thr_num=calcs[0].streamnum, .seg_num=cur_seg_prop_num};
    for(i=1; i<calcnum-1; i++){
        cur_seg_prop_num=(long int)round(prop_factor*calcs[i].streamnum);
        tasks[i]=(task_t){.lend=tasks[i-1].rend, .rend=tasks[i].lend+cur_seg_prop_num*segm, 
            .thr_num=calcs[i].streamnum, .seg_num=cur_seg_prop_num};
    }
    cur_seg_prop_num=(long int)(b-tasks[calcnum-2].rend)/segm;
    tasks[calcnum-1]=(task_t){.lend=tasks[calcnum-2].rend, .rend=b, 
        .thr_num=calcs[calcnum-1].streamnum, .seg_num=cur_seg_prop_num};
    
    for (i=0; i<calcnum; i++)
        send_task(&(confds[i]), &(tasks[i]), &(calcs[i].calc_addr));
    
    fprintf(stdout, "Completed\n");
    fflush(stdout);
    
    return confds;
}

void send_task(int *consockfd, task_t *task, struct sockaddr_in *addr)
{
    *consockfd=init_con_socket();
    if (connect(*consockfd, (struct sockaddr*)addr, sizeof(*addr))<0)
        handle_error("\nCannot connect to server. Timeout is over.", "Terminated");
        
    if (write(*consockfd, (void*)task, sizeof(*task))!=sizeof(*task))
        handle_error("\nProblems while writing into the socket. Timeout is over.", "Terminated");
        
    return;
}

double get_res(int calcnum, int *confds)
{
    fprintf(stdout, "Getting results from the calculators... ");
    fflush(stdout);
    
    int i;
    double inter_res;
    double res=0;
    
    for (i=0; i<calcnum; i++){
        if (read(confds[i], (void*)&inter_res, sizeof(inter_res))!=sizeof(inter_res))
            handle_error("\nProblems while reading from connection socket. Timeout is over.", "Terminated");
        res+=inter_res;
    }
    
    fprintf(stdout, "Completed\n");
    fflush(stdout);
    
    return res;
}

void set_clitimeo(int sockfd)
{
    struct timeval timeout={.tv_sec=CLI_RCV_TIMEOUT, .tv_usec=0};
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (void*)&timeout, sizeof(timeout))<0)
        handle_error("\nCannot set receive timeout", "Terminated");
    timeout.tv_sec=CLI_SND_TIMEOUT;
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (void*)&timeout, sizeof(timeout))<0)
        handle_error("Cannot set send timeout", "Terminated");
        
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
    exit(EXIT_FAILURE);
}
