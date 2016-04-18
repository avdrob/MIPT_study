#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

static char SET;
int CNT = 0;

void receive(pid_t);
void send(char*);
void rcvsig_hdlr(int);
void snd_hdlr(int);
void push_byte(char, pid_t);

int main(int argc, char *argv[]){
    sigset_t mask, omask;
    sigset_t oset;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, &omask);
    
    pid_t chpid = fork();
    
    if (chpid == 0){
        send(argv[1]);
    }
    else
        receive(chpid);
    
    return 0;
}

void receive(pid_t chpid){
    sigset_t mask;
    sigset_t oset;
    
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = rcvsig_hdlr;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGCHLD);
    sigaddset(&mask, SIGINT);
    act.sa_mask = mask;
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, 0);
    sigaction(SIGUSR2, &act, 0);
    sigaction(SIGCHLD, &act, 0);
    sigaction(SIGINT, &act, 0);
    
    sigemptyset(&mask);
    while (1){
        for (CNT = 0; CNT <= 7; CNT++){
          sigsuspend(&mask);  
          kill(chpid, SIGUSR1);
        }
        write(STDOUT_FILENO, (void*)(&SET), 1);
        sigsuspend(&mask);
        kill(chpid, SIGUSR2);
    }
}

void send(char *srcfile){
    int srcfd = open(srcfile, O_RDONLY);
    pid_t ppid = getppid();
    char buf;
    sigset_t mask;
    sigset_t oset;
    
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = snd_hdlr;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGCHLD);
    act.sa_mask = mask;
    sigaction(SIGUSR1, &act, 0);
    sigaction(SIGUSR2, &act, 0);
    sigemptyset(&mask);

    while (read(srcfd, (void*)(&buf), 1) > 0){
        push_byte(buf, ppid);
        kill(ppid, SIGINT);
        sigsuspend(&mask);
    }
    kill(ppid, SIGCHLD);
    close(srcfd);
    
    return;
}

void rcvsig_hdlr(int signo){
    char bitmask = '\0';
    switch (signo){
        case SIGUSR1:   
            bitmask = 1;
            bitmask = bitmask << CNT;
            bitmask = ~bitmask;
            SET &= bitmask;
            break;
        case SIGUSR2:
            bitmask = 1;
            bitmask = bitmask << CNT;
            SET |= bitmask;
            break;
        case SIGINT:
            break;
        case SIGCHLD:
            exit(EXIT_SUCCESS);
    }
    return;
}

void snd_hdlr(int signo){
    return;
}

void push_byte(char buf, pid_t ppid){
    int i;
    sigset_t mask;
    sigemptyset(&mask);

    for (i = 0; i <= 7; i++){
        if (((buf >> i) & 0x01) == 0)
            kill(ppid, SIGUSR1);
        else
            kill(ppid, SIGUSR2);
        sigsuspend(&mask);
    }
    return;
}
