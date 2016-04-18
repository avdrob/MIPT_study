#include "../Headers/server.h"
#include "../Headers/integral.h"

int main(int argc, char* argv[])
{    
    if (argc!=3)
        fatal_err(usage);
        
    fprintf(stdout, "Starting server...\n");
    fflush(stdout);
        
    int lissockfd=init_lis_socket(conv_addr(argv[1]));
    calcmsg_t msg=form_msg(lissockfd, conv_streamnum(argv[2]));
    
    int reqsockfd=init_req_socket();
    
    int consockfd;
    struct sockaddr_in cliaddr;
    socklen_t clilen=sizeof(cliaddr);
    
    fprintf(stdout, "Initialization section is completed. ");
    fflush(stdout);
    
    int chpid;
    while(1){
        chpid=fork();
        if (chpid==0){
            fprintf(stdout, "Server is ready\n");
            fflush(stdout);
            
            get_request(reqsockfd, msg);
            task_t task=get_task(lissockfd, &consockfd, &cliaddr, &clilen);
            double res=calculate(task.lend, task.rend, func, task.thr_num, task.seg_num);
            send_res(consockfd, res);
            exit(0);
        }
        else waitpid(chpid, NULL, 0);
    }
    
    return 0;
}
