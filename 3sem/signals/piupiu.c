#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int n=atoi(argv[0]);
    typedef struct{
        int pipefd[2];
        pid_t chpid;
    }children;
    
    children child[n];
        
	int i;

	for(i=0; i<n-1; i++){
		pipe(child[i].pipefd);
		child[i].chpid=fork();

        if(child[i].chpid==0)
        {
             
		
		if(chpid==0){
            int cnt;
            for(cnt=0; cnt<i; cnt++){
                close(pipefd[i][0]);
                close(pipefd[i][1]);
            }
        

        }
		

    }

	return 0;
}
