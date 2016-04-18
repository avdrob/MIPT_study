#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

#define MAXPROC 100
#define BUFSIZE 1024

const char *fifo="./fifo";
const char *list="./list";

void terminate(char*, int);
void clear_wrkspace(void);

int main(int argc, char *argv[]){
    //clear_wrkspace();
    
    if (argc == 2){
        typedef struct{
            pid_t pid;
            int connectfd;
        } rcvr_t;

        typedef struct{
            int rcvnum;
            int tail;
            int head;
            rcvr_t list[MAXPROC];
        } queue_t;  
        
        char *srcfile=argv[1];
        queue_t queue={.rcvnum=0, .tail=0, .head=0};
        int rdlfd;
        int outpfd;
        int rdsrc;
        void *buf[BUFSIZE];
        int ret_val;
        char bufstr[40];
        
        if (mkfifo(fifo, 0666) < 0)
            terminate("Cannot create file ./fifo: ", errno);
        if (mkfifo(list, 0666) < 0)
            terminate("Cannot create file ./list: ", errno);
        if ((rdlfd=open(list, O_RDONLY)) < 0)
            terminate("Cannot open file ./list: ", errno);
        
        do{
            while ((ret_val=read(rdlfd, (void*)&(queue.list[queue.tail].pid), sizeof(pid_t))) > 0){
                char pid_str[10];
                char pid_fifo[15]="./tmp/";
                
                sprintf(pid_str, "%d", queue.list[queue.tail].pid);
                strcat((char*)pid_fifo, pid_str);
                
                if ((queue.list[queue.tail].connectfd=open(pid_fifo, O_RDWR)) < 0){
                    strcpy((char*)bufstr, "Cannot open process-specific file ");
                    strcat((char*)bufstr, pid_fifo);
                    terminate (bufstr, errno);
                }
                if (queue.tail != (queue.head-2)){
                    queue.tail++;
                    queue.rcvnum++;
                }
            } 
            if (ret_val < 0)
                terminate("Cannot read from file ./list: ", errno);
  
            if (queue.rcvnum == 0){
                if (close(rdlfd) < 0)
                    terminate("Cannot close file ./list: ", errno);
                if (unlink(list) < 0)
                    terminate("Cannot remove file ./list: ", errno);
                if (unlink(fifo) < 0)
                    terminate("Cannot remove file ./fifo: ", errno);
                break;
            }
            
            while (queue.rcvnum > 0){   
                int bytesnum;
                char msg;   //"OK"
                
                if ((outpfd=open(fifo, O_RDWR)) < 0)
                    terminate ("Cannot open file ./fifo: ", errno);
                    
                if ((rdsrc=open(srcfile, O_RDONLY)) < 0){
                    strcpy((char*)bufstr, "Cannot open file ");
                    strcat((char*)bufstr, argv[1]);
                    terminate (bufstr, errno);
                }
                msg='O';   //"OK"
                if (write(queue.list[queue.head].connectfd, &msg, 1) < 0)
                    terminate("Cannot write into process-specific file: ", errno);
                
                while ((bytesnum=read(rdsrc, buf, BUFSIZE)) > 0){
                    if (write(outpfd, buf, bytesnum) < 0)
                        terminate("Cannot write into ./fifo file", errno);
                }
                if (bytesnum < 0){
                    strcpy((char*)bufstr, "Cannot read from file ");
                    strcat((char*)bufstr, argv[1]);
                    terminate (bufstr, errno);
                }
                
                close(outpfd);
                close(rdsrc);
                
                if (read(queue.list[queue.head].connectfd, &msg, 1) < 0)
                    terminate("Cannot read from process-specific file: ", errno);
                    
                if (msg=='C'){    //"Completed"
                    close(queue.list[queue.head].connectfd);
                    queue.list[queue.head].pid=queue.list[queue.head].connectfd=0;
                    queue.rcvnum--;
                    queue.head++;
                }
            }
        }while(1);
    }
    
    else if (argc == 1){
        int wrlfd;
        pid_t pid;
        char pid_str[10];
        char pid_fifo[15]="./tmp/";
        int connectfd;
        void *buf[BUFSIZE];
        char msg;
        int inpfd;
        int bytesnum;
        char bufstr[40];
        
        if ((wrlfd=open(list, O_WRONLY)) < 0)
            terminate("Cannot open file ./list: ", errno);
            
        pid=getpid();
        sprintf(pid_str, "%d", pid);
        strcat((char*)pid_fifo, pid_str);
        
        if (mkfifo(pid_fifo, O_CREAT| O_EXCL | 0666) < 0){
            strcpy((char*)bufstr, "Cannot create file ");
            strcat((char*)bufstr, (char*)pid_fifo);
            terminate (bufstr, errno);
        }
        if ((connectfd=open(pid_fifo, O_RDWR) < 0)){
            strcpy((char*)bufstr, "Cannot open file ");
            strcat((char*)bufstr, (char*)pid_fifo);
            terminate (bufstr, errno);
        }
        if (write(wrlfd, (void*)&pid, sizeof(pid_t)) < 0)
            terminate("Cannot write into process-specific file", errno);
        close(wrlfd);
        
        read(connectfd, &msg, 1);
        if (msg=='O'){    //"OK"
            if ((inpfd=open(fifo, O_RDONLY) < 0))
                terminate("Cannot open file ./fifo ", errno);
                
            while ((bytesnum=read(inpfd, buf, BUFSIZE)) > 0){
                if (write(STDOUT_FILENO, buf, bytesnum) < 0)
                    terminate("Cannot write into stdout", errno);
                }
            if (bytesnum < 0)
                terminate("Canot read from file ./fifo ", errno);
                
            msg='C';    //"Completed"
            if (write(connectfd, &msg, 1) < 0)
                terminate("Cannot write into process-specific file ", errno);
            close(inpfd);
        }
            
        close(connectfd);
        if (unlink(pid_fifo) < 0)
            terminate("Cannot remove process-specific file", errno);
    }
    
    else{
        printf("Wrong arguments\n");
        printf("Usage:\n\
        \"%s SRCFILE\" for sender mode\n\
        \"%s\" for receiver mode\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void terminate(char *errstr, int errnum){
    perror(errstr);
    strerror(errnum);
    exit(EXIT_FAILURE);
}

void clear_wrkspace(void){
    pid_t rmpid=fork();
    if (rmpid == 0){
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        execl("/bin/bash", "bin/bash", "-c", "./clear", NULL);
        printf("Warning: workspace is not cleared\n\n");
        /*unlink(fifo);
        unlink(list);*/
    }
    wait(NULL);
}

