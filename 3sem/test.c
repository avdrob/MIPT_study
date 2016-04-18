#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define KEY 		1
#define PERM		0666
#define NO_ERR 		0
#define SEND_ERR	1
#define REC_ERR		2
#define BUFSIZE 	1024

#define ASSERT(cond) do {\
    if (!(cond)) {\
	printf ("Assert in %s on line %d. errno = %s.\n"\
	, __FILE__, __LINE__, strerror(errno));\
	exit(1);\
    }\
} while (0)

struct sembuf gen_lock_send[2] 	=  {0, 0, 0,
				    0, 1, SEM_UNDO};

struct sembuf gen_ini_send[3] 	=  {1, 1, SEM_UNDO,
				    3, 10, 0,
				    3, -10, SEM_UNDO};

struct sembuf gen_lock_rec[5]	=  {1, -1, 0,
				    0, 1, SEM_UNDO,
				    2, 1, 0,
				    2, 10, 0,
				    2, -10, SEM_UNDO};

struct sembuf ind_lock_send[1] 	=  	{2, -1, 0};
struct sembuf ind_lock_rec[1] 	=  	{3, -1, 0};
struct sembuf ind_unlock_send[1]=  	{2, 1, 0};
struct sembuf ind_unlock_rec[1] =  	{3 ,1, 0};

struct sembuf ind_check_rec[1]  =   {2, 0, IPC_NOWAIT};
struct sembuf ind_check_send[1] =   {3, 0, IPC_NOWAIT};

typedef struct msg_t {
    int end_file;
    int bytes_read;
    char buf[BUFSIZE];
} msg_t;

int send (char *argv[], int semid, int shmid, msg_t *shm);
int rec (int semid, int shmid, msg_t *shm);
void exit_p (int error, int semid, int shmid);

int main(int argc, char *argv[]) 
{
    
    key_t key = ftok (argv[0], KEY);
    ASSERT (key != -1);
    
    int semid = semget (key, 4, PERM|IPC_CREAT);
    ASSERT (semid != -1);
    
    int shmid = shmget (key, sizeof (msg_t), PERM|IPC_CREAT);
    ASSERT (shmid != -1);

    if (argc == 2) {
	ASSERT (!semop (semid, gen_lock_send, 2));
	ASSERT (!semctl (semid, 2, SETVAL, 0));
	ASSERT (!semctl (semid, 3, SETVAL, 0));

	ASSERT (!semop (semid, gen_ini_send, 3));

	msg_t *shm = shmat (shmid, 0, 0);
	ASSERT ((void*) shm != (void*) -1);
	send (argv, semid, shmid, shm);

	
    } else if (argc == 1) {
	
	ASSERT (!semop (semid, gen_lock_rec, 5));
	msg_t *shm = shmat (shmid, 0, SHM_RDONLY);
	ASSERT ((void*) shm != (void*) -1);
	rec (semid, shmid, shm);

	exit_p (NO_ERR, semid, shmid);	
    } else 
	ASSERT (0);

    return 0;
}

int send (char *argv[], int semid, int shmid, msg_t* shm)
{
    int file_inp = open (argv[1], O_RDONLY);
    ASSERT (file_inp != -1);
    
    struct msg_t msg;
    msg.end_file = 0;
    msg.bytes_read = 0;
    
    while (!msg.end_file) {
	msg.bytes_read = read (file_inp, msg.buf, BUFSIZE);
	ASSERT (msg.bytes_read != -1);
	if (msg.bytes_read < BUFSIZE)
		    msg.end_file = 1;
	
	ASSERT (!semop (semid, ind_lock_send, 1));

	if (semop (semid, ind_check_rec, 1) == -1)
	    exit_p (REC_ERR, semid, shmid);
	*shm = msg;
	
	ASSERT (!semop (semid, ind_unlock_rec, 1));
    }
    
    ASSERT (!shmdt (shm));
    ASSERT (!close (file_inp));
    return 0;
}

int rec (int semid, int shmid, msg_t* shm)
{
    struct msg_t msg;
    msg.end_file = 0;
    msg.bytes_read = 0;
    
    while (!msg.end_file) {
	ASSERT (!semop (semid, ind_lock_rec, 1));
	
	msg = *shm;	
	if (!msg.end_file && semop (semid, ind_check_send, 1) == -1) 
	    exit_p (SEND_ERR, semid, shmid);
	
	ASSERT (!semop (semid, ind_unlock_send, 1));

	write (STDOUT_FILENO, msg.buf, msg.bytes_read);
    }
    
    ASSERT (!shmdt (shm));
    return 1;
}

void exit_p (int error, int semid, int shmid)
{
    
    ASSERT (!shmctl (shmid, IPC_RMID, 0));
    ASSERT (!semctl (semid, 0, IPC_RMID));
    if (error == SEND_ERR) {
	printf ("Problems in send.\n");
	exit (1);
    } else if (error == REC_ERR) {
	printf ("Problems in rec.\n");
	exit (1);
    }

    exit (0);
}