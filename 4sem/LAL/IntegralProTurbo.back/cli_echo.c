#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define PORT 15000

const char *checkstr="I do need some calculation";
const char *brd="127.255.255.255";

int main()
{
    int sockfd, brdcast, len;
    struct sockaddr_in servaddr, cliaddr;
    char buf [100];
    
    brdcast=1;
    sockfd=socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &brdcast, sizeof(brdcast));
    bzero((void*)&servaddr, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    //servaddr.sin_addr.s_addr=htonl(INADDR_BROADCAST);
    inet_pton(AF_INET, brd, (void*)&servaddr.sin_addr);
    
    bzero((void*)&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family=AF_INET;
    cliaddr.sin_port=htons(0);
    cliaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    bind(sockfd, (struct sockaddr*)&cliaddr, (socklen_t)sizeof(cliaddr));
    //printf("%d\n", checkstr[(int)strlen(checkstr)]);
    sendto(sockfd, (void*)checkstr, strlen(checkstr)+1, 0, (struct sockaddr*)&servaddr, (socklen_t)sizeof(servaddr));
    recvfrom(sockfd, &buf, 100, 0, NULL, NULL);
    
    printf("%s\n", buf);
    
    close(sockfd);
    
    return 0;
}
