#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PORT 15000

const char *checkstr="I do need some calculation";
const char *respstr="Yes, my captain!";

int init_req_socket(void);
void handle_error(const char*);

int main(int argc, char* argv[])
{
    int sockfd=init_req_socket();
    
    return 0;
}

int init_req_socket()
{
    int reuse=1;
    int sockfd;
    
    if (sockfd=socket(AF_INET, SOCK_DGRAM, 0)<0)
        handle_error("Cannot create request socket\n");
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))<0)
        handle_error("Cannot enable SO_REUSEADDR option\n");
    
    return sockfd;
}

void handle_error(const char *err_msg)
{
    perror(err_msg);
    printf("%s\n", strerror(errno));
    
    exit(EXIT_FAILURE);
}
    
    
