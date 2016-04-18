#include "../Headers/integral.h"

double calculate (double a, double b, double (*func)(double), int thr_num, long int seg_num)
{ 
    fprintf(stdout, "Calculating... ");
    fflush(stdout);
    
    pthread_t *threads=(pthread_t*)calloc(thr_num, sizeof(pthread_t));
    calcarg_t *args=(calcarg_t*)calloc(thr_num, sizeof(calcarg_t));
    if (!threads | !args)
        handle_error("\nCannot allocate memory\n", "Resarting...\n");
    
    int i;
    for (i=0; i<thr_num; i++){
        args[i].f=func;
        args[i].thr_num=thr_num;
        args[i].a=a;
        args[i].b=b;
        args[i].num=i;
        args[i].seg_num=seg_num;
        if (pthread_create(&threads[i], NULL, calc_routine, (void*)(&args[i]))!=0)
            fatal_err("Cannot create thread. Restarting...\n");
    }
    
    double I=0, *val=0;
    for (i=0; i<thr_num; i++){
        if (pthread_join(threads[i], (void**)(&val))!=0)
            fatal_err("Cannot join with thread. Restarting...\n");
        I+=*val;
    }
    free(threads);
    free(args);
    
    fprintf(stdout, "Completed\n");
    fflush(stdout);
    
    return I;
}

void *calc_routine(void *arg_)
{   
    calcarg_t arg=*((calcarg_t*)arg_);
    int seg_num=arg.seg_num;
    int thr_num=arg.thr_num;
    double segm=(arg.b-arg.a)/seg_num;
    double x1, x2;
    
    double *val=(double*)calloc(1, sizeof(double));
    double sum=0, buf;
    if (!val)
        fatal_err("Cannot allocate memory. Restarting...\n");
    int i;
    for (i=arg.num; i<seg_num; i+=thr_num){
        x1=arg.a+segm*i;
        x2=x1+segm;
        buf=(segm/6)*(arg.f(x1)+4*arg.f((x1+x2)/2)+arg.f(x2));
        sum+=buf;
    }
    *val=sum;
    pthread_exit((void*)val);
}
