#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 15000
#define BACKLOG 10

const char *checkstr="I do need some calculation";
const char *respstr="Yes, my captain!";

int main(int argc, char *argv[])
{
    int listenfd, confd, clilen, bytenum, so;
    struct sockaddr_in servaddr, cliaddr;
    char buf[100];
    
    bzero((void*)&servaddr, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    //inet_pton(AF_INET, argv[1], (void*)&servaddr.sin_addr);
    
    
    listenfd=socket(AF_INET, SOCK_DGRAM, 0);
    so=1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &so, sizeof(so));
    if (bind(listenfd, (struct sockaddr*)&servaddr, (socklen_t)sizeof(servaddr))<0){
        printf("Cannot bind\n");
        exit(EXIT_FAILURE);
    }
    clilen=sizeof(cliaddr);
    recvfrom(listenfd, &buf, 100, 0, (struct sockaddr*)&cliaddr, &clilen);
    sendto(listenfd, (void*)respstr, strlen(respstr)+1, 0, (struct sockaddr*)&cliaddr, (socklen_t)sizeof(cliaddr));

    close(listenfd);
    
    return 0;
}
