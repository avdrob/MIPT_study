#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define PORT 15000
#define MAXSTRLEN 50
#define MAXCALCSNUM 255

typedef struct calcmsg_t{
    char respstr[MAXSTRLEN];
    struct sockaddr_in calc_addr;
    int streamnum;
} calcmsg_t;

const char *reqstr="I do need some calculation";
const char *respstr="Yes, my captain!";

void handle_error(const char*);
int init_brd_socket(void);
calcmsg_t *detect_calcs(int, int*);

int main(int argc, char *argv[])
{
    int calcnum=conv_num(argv[1]);
    int sockfd=init_brd_socket();
    int streamnum=0;
    
    calcmsg_t *calcs=detect_calcs(sockfd, &calcnum);

    return 0;
}

int init_brd_socket()
{
    int brdcast=1;
    int sockfd;
    
    if (sockfd=socket(AF_INET, SOCK_DGRAM, 0)<0)
        handle_error("Cannot create broadcast socket\n");
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &brdcast, sizeof(brdcast))<0)
        handle_error("Cannot enable SO_BROADCAST option\n");
        
    return sockfd;
}
        
int conv_num(char *str)
{
    char *endptr;
    int n=(int)strtol(str, (char**)&endptr, 10);
    if (!((endptr[0]==' ') | (endptr[0]=='\n') | (endptr[0]=='\0')))
        handle_error("The first arg is wrong\nMin number of calculators must be positive integer\n");
        
    return n;
}

calcmsg_t *detect_calcs(int sockfd, int *calcnum)
{
    calcmsg_t *calcs=(calcmsg_t*)calloc(MAXCALCSNUM, sizeof(calcmsg_t));
    if (calcs==NULL)
        handle_error("Cannot allocate memory\n");
        
    struct sockaddr_in servaddr={.sin_family=AF_INET, .sin_port=htons(PORT), .sin_addr.s_addr=htonl(INADDR_BROADCAST)};
    struct sockaddr_in cliaddr={.sin_family=AF_INET, .sin_port=htons(0), .sin_addr.s_addr=htonl(INADDR_ANY)};
    
    if (bind(sockfd, (struct sockaddr*)&cliaddr, (socklen_t)sizeof(cliaddr))<0)
        handle_error("Cannot bind client address\n");
    
    if (sendto(sockfd, (void*)reqstr, strlen(reqstr)+1, 0, (struct sockaddr*)&servaddr, (socklen_t)sizeof(servaddr))<0)
        handle_error("Cannot send broadcast message\n");
        
    int cnt=0;
    while (cnt<(*calcnum)){
        if ((recvfrom(sockfd, (void*)&(calcs[cnt]), sizeof(calcs[0]), MSG_WAITALL, NULL, NULL)==sizeof(calcs[0])) &&
            (strcmp((char*)calcs[cnt].respstr, respstr)==0) && (cnt<MAXCALCSNUM))
            cnt++;
    }
    
    if ((recvfrom(sockfd, (void*)&(calcs[cnt]), sizeof(calcs[0]), MSG_DONTWAIT, NULL, NULL)==sizeof(calcs[0])) && 
        (strcmp((char*)calcs[cnt].respstr, respstr)==0) && (cnt<MAXCALCSNUM)) {
        cnt++;
        while (1){
            if ((recvfrom(sockfd, (void*)&(calcs[cnt]), sizeof(calcs[0]), MSG_DONTWAIT, NULL, NULL)==sizeof(calcs[0])) &&
                (strcmp((char*)calcs[cnt].respstr, respstr)==0) && (cnt<MAXCALCSNUM))
                cnt++;
            else break;
        }
    }
    *calcnum=cnt;
    
    return calcs;
}

void handle_error(const char *err_msg)
{
    perror(err_msg);
    printf("%s\n", strerror(errno));
    
    exit(EXIT_FAILURE);
}

        
