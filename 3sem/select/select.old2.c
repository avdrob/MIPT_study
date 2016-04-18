#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct{
    int readfd[2];
    int writefd[2];
    void* buf;
    int bufsize;
    pid_t chpid;
    int chnum;
}child_t;

void creat_chld(int, child_t*);
void go_away(child_t, char*);


int main(int argc, char *argv[]){
    int n = atoi(argv[2]);
    fd_set readset, writeset;
    child_t chset[n];
    
    int i;
    for (i = 0; i < n; i++){
        creat_chld(i, chset);
        if (chset[i].chpid == 0)
            go_away(chset[i], argv[1]);
    }
    
    int bytesnum;
    int maxfdp1 = chset[n-1].writefd[1] + 2;
    struct timeval nowait = {0, 0};
    while (1){
        FD_ZERO(&writeset);
        FD_ZERO(&readset);
        for (i = 0; i < n - 1; i++){
            printf("BUBUBU\n");
            fflush(stdout);
            FD_ZERO(&writeset);
            FD_ZERO(&readset);
            FD_SET(chset[i].readfd[0], &readset);
            select(maxfdp1, &readset, NULL, NULL, NULL);
            bytesnum = read(chset[i].readfd[0], chset[i].buf, chset[i].bufsize);
            FD_SET(chset[i+1].writefd[1], &writeset);
            write(chset[i+1].writefd[1], chset[i].buf, bytesnum);
        }
        FD_ZERO(&readset);
        FD_SET(chset[n-1].readfd[0], &readset);
        bytesnum = read(chset[n-1].readfd[0], chset[n-1].buf, chset[n-1].bufsize);
        write(STDOUT_FILENO, chset[n-1].buf, bytesnum);
        
        if (bytesnum == 0)
            exit(EXIT_SUCCESS);
    }
}
    
void creat_chld(int chldno, child_t* chldset){
    pipe(chldset[chldno].readfd);
    pipe(chldset[chldno].writefd);
    close(chldset[chldno].readfd[1]);
    close(chldset[chldno].writefd[0]);
    chldset[chldno].bufsize = 1111 * (chldno + 1);
    chldset[chldno].buf = calloc(chldset[chldno].bufsize, sizeof(char));
    chldset[chldno].chnum = chldno;
    chldset[chldno].chpid = fork();
    return;
}

void go_away(child_t data, char *srcfile){
    close(data.readfd[0]);
    close(data.writefd[1]);
    
    int srcfd;
    int bytesnum;
    
    switch (data.chnum){
        case 0:
            close(data.writefd[0]);
            srcfd = open(srcfile, O_RDONLY);
            while (1){
                bytesnum = read(srcfd, data.buf, data.bufsize);
                write(data.readfd[1], data.buf, bytesnum);
                if (bytesnum == 0){
                    close(data.readfd[1]);
                    close(srcfd);
                    free(data.buf);
                    exit(EXIT_SUCCESS);
                }
            }
        default:
            while (1){
                bytesnum = read(data.writefd[0], data.buf, data.bufsize);
                write(data.readfd[1], data.buf, bytesnum);
                if (bytesnum == 0){
                    close(data.readfd[1]);
                    close(data.writefd[0]);
                    free(data.buf);
                    exit(EXIT_SUCCESS);
                }
            }
    }
}
