#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE 1024
#define PROJ_ID 1

typedef struct{
    int eof;
    int bytecnt;
    char buf[BUF_SIZE];
} shm_t;

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO*/
};


struct sembuf proc_wait[3]={4, 0, IPC_NOWAIT,
                            1, -1, 0,
                            4, 0, IPC_NOWAIT};
                            
struct sembuf proc_start[3]={3, 0, IPC_NOWAIT,
                             1, 1, 0,
                             3, 0, IPC_NOWAIT};
                             
struct sembuf mem_lock_snd[4]={3, 0, IPC_NOWAIT,
                               0, 0, 0,
                               0, 1, 0,
                               3, 0, IPC_NOWAIT};
                               
struct sembuf mem_lock_rcv[4]={4, 0, IPC_NOWAIT,
                               0, 0, 0,
                               0, 1, 0,
                               4, 0, IPC_NOWAIT};                               
                           
struct sembuf mem_unlock_snd[3]={3, 0, IPC_NOWAIT,
                                 0, -1, 0,
                                 3, 0, IPC_NOWAIT};
                                 
struct sembuf mem_unlock_rcv[3]={4, 0, IPC_NOWAIT,
                                 0, -1, 0,
                                 4, 0, IPC_NOWAIT};                                 

struct sembuf call_rcv[3]={3, 0, IPC_NOWAIT,
                           2, -1, 0,
                           3, 0, IPC_NOWAIT};
                           
struct sembuf call_snd[3]={4, 0, IPC_NOWAIT,
                           2, 1, 0,
                           4, 0, IPC_NOWAIT};
                           
struct sembuf set_rcv[2]={3, 1, 0,
                          3, -1, SEM_UNDO};
                          
struct sembuf set_snd[2]={4, 1, 0, 
                          4, -1, SEM_UNDO};
                          
struct sembuf check_snds[2]={6, 0, IPC_NOWAIT,
                             6, 1, SEM_UNDO};
                             
struct sembuf check_rcvs[2]={5, 0, IPC_NOWAIT,
                             5, 1, SEM_UNDO};
             
struct sembuf set_presense[1]={7, 1, SEM_UNDO};                                  
struct sembuf chk_presence[2]={7, -1, IPC_NOWAIT,
                               7, 1, 0};
                                
unsigned short zr_set[8]={0, 0, 0, 0, 0, 0, 0, 0};
union semun clear_set;
                                                  
const char *path="./keyfile";

void send(char*);
void receive(void);
void get_out(int, int);
void unwanted(void);
void clear(int);

int main(int argc, char *argv[])
{
    clear_set.array=zr_set;
    if (argc==1)
        receive();
    else if (argc==2)
        send(argv[1]);
    
    fprintf(stderr, "\nWrong args\n");
    exit(EXIT_FAILURE);
}

void receive(void)
{
    key_t key=ftok(path, PROJ_ID);
    int semid;
    int shmid;
    semid=semget(key, 8, IPC_CREAT|0666);
    shmid=shmget(key, sizeof(shm_t), IPC_CREAT|0666);
    
    if (semop(semid, &check_rcvs[0], 2)<0)
        unwanted();
    
    if (semop(semid, &chk_presence[0], 2)<0)
        semctl(semid, 0, SETALL, clear_set);
    semop(semid, &set_presense[0], 1);
    shm_t *shmptr=(shm_t*)shmat(shmid, 0, 0);
    semop(semid, &set_rcv[0], 2);
    
    abort();
        
    int bytesnum=0;
    int offs=0;
    while (1){
        if (semop(semid, &proc_wait[0], 3)<0)
            get_out(semid, shmid);
        if (semop(semid, &mem_lock_rcv[0], 4)<0)
            get_out(semid, shmid);
            
        while (shmptr->bytecnt!=0){
            bytesnum=write(STDOUT_FILENO, shmptr->buf+offs, shmptr->bytecnt);
            if (bytesnum<0 || (bytesnum==0 && shmptr->bytecnt!=0))
                get_out(semid, shmid);
            shmptr->bytecnt-=bytesnum;
            offs+=bytesnum;
        }
        offs=0;
            
        if (shmptr->eof==1)
            break;
        if (semop(semid, &(mem_unlock_rcv[0]), 3)<0)
            get_out(semid, shmid);
        if (semop(semid, &call_snd[0], 3)<0)
            get_out(semid, shmid);
    }
    semop(semid, &call_snd[0], 3);
    shmdt(shmptr);
    exit(EXIT_SUCCESS);
}

void send(char *srcfile)
{
    int srcfd=open(srcfile, O_RDONLY);
    key_t key=ftok(path, PROJ_ID);
    int shmid;
    int semid;
    
    semid=semget(key, 8, IPC_CREAT|0666);
    shmid=shmget(key, sizeof(shm_t), IPC_CREAT|0666);
    
    if (semop(semid, &check_snds[0], 4)<0){
        if (semop(semid, &chk_presence[0], 2)<0)
            semctl(semid, 0, SETALL, clear_set);
        
        
        
    semop(semid, &set_presense[0], 1);
    semop(semid, &set_snd[0], 2);
    shm_t *shmptr=(shm_t*)shmat(shmid, 0, 0);
    
    
    
    shmptr->eof=0;
    int bytesnum;
    while(1){
        if (semop(semid, &mem_lock_snd[0], 4)<0)
            get_out(semid, shmid);
        if (semop(semid, &proc_start[0], 3)<0)
            get_out(semid, shmid);
            
        shmptr->bytecnt=read(srcfd, shmptr->buf, BUF_SIZE);
        if (shmptr->bytecnt==0){
            shmptr->eof=1;
            break;
        }
        else if (shmptr->bytecnt<0)
            get_out(semid, shmid);
            
        if (semop(semid, &mem_unlock_snd[0], 3)<0)
            get_out(semid, shmid);
        if (semop(semid, &call_rcv[0], 3)<0)
            get_out(semid, shmid);
    }
    if (semop(semid, &mem_unlock_snd[0], 3)<0)
        get_out(semid, shmid);
    semop(semid, &call_rcv[0], 3);
    
    shmdt(shmptr);
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID);
    exit(EXIT_SUCCESS);
}

void get_out(int semid, int shmid)
{
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID);
    fprintf(stderr, "\nFATAL ERROR\n");
    exit(EXIT_FAILURE);
}

void unwanted(void)
{
    fprintf(stderr, "\nI'm unwanted here\n");
    exit(EXIT_FAILURE);
}

/*void clear_sem(int semid)
{
    if (semctl(semid, 0, IPC_RMID)<0){
        //printf("HHOOOA\n");
        fprintf(stderr, "\nFATAL ERROR\n");
        exit(EXIT_FAILURE);
    }
    return;
}

void clear_shm(int shmid)
{
    if (shmctl(shmid, IPC_RMID, 0)<0){
        fprintf(stderr, "\nFATAL ERROR\n");
        exit(EXIT_FAILURE);
    }
    return;
}*/

void clear(int key)
{
    int semid=semget(key, 7, IPC_CREAT|0666);
    int shmid=shmget(key, sizeof(shm_t), IPC_CREAT|0666);
    shmctl(shmid, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID);
    
    return;
}
    
