#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct{
    int readfd[2];
    int writefd[2];
    void* buf;
    int chnum;
    int rdvalid;
    int wrvalid;
    int bufsize;
    int bytecnt;
    int offs;
}child_t;


void co_work(child_t*, char*);
void fillset(fd_set*, fd_set*, child_t[], int);
void close_unused(child_t*, int);
void get_out(void);

int main(int argc, char *argv[])
{
    int n=atoi(argv[2]);
    fd_set readset, writeset;
    int bytesnum;
    child_t chset[n];
    memset(chset, 0x0, n*sizeof(child_t));
    
    int i;
    int pid;
    for (i=0; i<n; i++){
        chset[i].bufsize=1111*(i+1);
        pipe(chset[i].readfd);
        pipe(chset[i].writefd);
        chset[i].chnum=i;
        chset[i].rdvalid=1;
        chset[i].wrvalid=1;
        chset[i].bytecnt=0;
        chset[i].offs=0;
        pid=fork();
        chset[i].buf=calloc(chset[i].bufsize, sizeof(char));
        
        if (pid>0){
            close(chset[i].readfd[1]);
            close(chset[i].writefd[0]);
            fcntl(chset[i].readfd[0], F_SETFL, O_NONBLOCK);
            fcntl(chset[i].writefd[1], F_SETFL, O_NONBLOCK);
        }
        
        else if (pid==0){
            close_unused(chset, i);
            co_work(&(chset[i]), argv[1]);
        }
    }
    close(chset[0].writefd[1]);
    
    int maxfdp1=chset[n-1].writefd[1] + 1;
    while (1){
        if (chset[n-1].bytecnt>0){
            bytesnum=write(STDOUT_FILENO, chset[n-1].buf+chset[n-1].offs, chset[n-1].bytecnt);
            if (bytesnum<0)
                get_out();
            chset[n-1].bytecnt-=bytesnum;
            chset[n-1].offs+=bytesnum;
            if (chset[n-1].offs==chset[n-1].bufsize)
                chset[n-1].offs=0;
            continue;
        }
        else if (fcntl(chset[n-1].readfd[0], F_GETFL, NULL)==-1)
            exit(EXIT_SUCCESS);
        
        fillset(&readset, &writeset, chset, n);
        select(maxfdp1, &readset, &writeset, NULL, NULL);
        for (i=0; i<n; i++){
            if (FD_ISSET(chset[i].readfd[0], &readset)){
                bytesnum=read(chset[i].readfd[0], chset[i].buf+chset[i].offs+chset[i].bytecnt, 
                                                chset[i].bufsize-chset[i].bytecnt-chset[i].offs);
                if (bytesnum<0)
                    get_out();
                if (bytesnum==0){
                    close(chset[i].readfd[0]);
                    chset[i].rdvalid=0;
                }
                else
                    chset[i].bytecnt+=bytesnum;
            }
        }
        
        for (i=1; i<n; i++){
            if (FD_ISSET(chset[i].writefd[1], &writeset))
                if (chset[i-1].bytecnt>0){
                    bytesnum=write(chset[i].writefd[1], chset[i-1].buf+chset[i-1].offs, chset[i-1].bytecnt);
                    if (bytesnum<0)
                        get_out();
                    chset[i-1].bytecnt-=bytesnum;
                    chset[i-1].offs+=bytesnum;
                    //if (chset[i-1].offs==chset[i-1].bufsize)
                    if (chset[i-1].bytecnt==0)
                        chset[i-1].offs=0;
                }
                else if (chset[i-1].bytecnt==0 && fcntl(chset[i-1].readfd[0], F_GETFL, NULL)==-1){
                    close(chset[i].writefd[1]);
                    chset[i].wrvalid=0;
                }
        }
    }
}

void co_work(child_t *data, char *srcfile)
{
    int bytesnum;
    if (data->chnum==0){
        close(data->writefd[0]);
        data->writefd[0]=open(srcfile, O_RDONLY);
    }
    
    while (1){
        bytesnum=read(data->writefd[0], data->buf+data->offs+data->bytecnt, 
                                    data->bufsize-data->offs-data->bytecnt);
        if (bytesnum==0 && data->bytecnt==0)
            exit(EXIT_SUCCESS);
        else if (bytesnum<0)
            get_out();
        if (data->chnum==3)
            sleep(1);
        data->bytecnt+=bytesnum;
        bytesnum=write(data->readfd[1], data->buf+data->offs, data->bytecnt);
        if (bytesnum<0)
            get_out();
        data->bytecnt-=bytesnum;
        data->offs+=bytesnum;
        if (data->offs==data->bufsize)
            data->offs=0;
    }
}
        
void fillset(fd_set *readset, fd_set *writeset, child_t chset[], int n)
{
    FD_ZERO(readset);
    FD_ZERO(writeset);
    int i;
    if (chset[0].rdvalid && (chset[0].offs+chset[0].bytecnt<chset[0].bufsize))
        FD_SET(chset[0].readfd[0], readset);
    for (i=1; i<n; i++){
        if (chset[i].rdvalid && (chset[i].offs+chset[i].bytecnt<chset[i].bufsize))
            FD_SET(chset[i].readfd[0], readset);
        if (chset[i].wrvalid && ((chset[i-1].bytecnt>0) || (fcntl(chset[i-1].readfd[0], F_GETFL, NULL)==-1)))
            FD_SET(chset[i].writefd[1], writeset);
    }
    return;
}

void close_unused(child_t chset[], int chldno)
{
    int i;
    for (i=0; i<chldno; i++){
        close(chset[i].readfd[0]);
        close(chset[i].writefd[1]);
    }
    close(chset[chldno].readfd[0]);
    close(chset[chldno].writefd[1]);
    
    return;
}

void get_out(void)
{
    fprintf(stderr, "\nFATAL ERROR\n");
    exit(EXIT_FAILURE);
}
