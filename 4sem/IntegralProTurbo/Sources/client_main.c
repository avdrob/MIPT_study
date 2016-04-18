#include "../Headers/client.h"

int main(int argc, char *argv[])
{
    if (argc!=5)
        fatal_err(usage);
    
    double a=conv_segmend(argv[1]);
    double b=conv_segmend(argv[2]);
    if (b<a)
        fatal_err(order);
    
    long int total_segnum=conv_segnum(argv[3]);
    int calcnum=conv_calcnum(argv[4]);
    
    int brdsockfd=init_brd_socket();
    calcmsg_t *calcs=detect_calcs(brdsockfd, &calcnum);
    close(brdsockfd);
    int *confds=distr_task(a, b, total_segnum, calcnum, calcs);
    double res=get_res(calcnum, confds);
    
    printf("%lf\n", res);

    return 0;
}
