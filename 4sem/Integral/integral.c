#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct arg{
    double (*f)(double);
    long int thr_num;
    double a;
    double b;
    int num;
    long int seg_num;
} arg_t;
    
void *start_routine(void*);
double calculate(double, double, double (*)(double), long int, long int);
void handle_error(char*);

double calculate (double a, double b, double (*func)(double), long int thr_num, long int seg_num)
{ 
        
    pthread_t *threads=(pthread_t*)calloc(thr_num, sizeof(pthread_t));
    arg_t *args=(arg_t*)calloc(thr_num, sizeof(arg_t));
    if (!threads | !args)
        handle_error("Cannot allocate memory\n");
    
    int i;
    for (i=0; i<thr_num; i++){
        args[i].f=func;
        args[i].thr_num=thr_num;
        args[i].a=a;
        args[i].b=b;
        args[i].num=i;
        args[i].seg_num=seg_num;
        if (pthread_create(&threads[i], NULL, start_routine, (void*)(&args[i]))<0)
            handle_error("Cannot create thread\n");
    }
    
    double I=0, *val=0;
    for (i=0; i<thr_num; i++){
        if (pthread_join(threads[i], (void**)(&val))<0)
            handle_error("Cannot join with thread\n");
        printf("\n%d. %lf", i, *val);
        fflush(stdout);
        I+=*val;
    }
    free(threads);
    free(args);
    return I;
}

void *start_routine(void *arg_)
{   
    arg_t arg=*((arg_t*)arg_);
    int seg_num=arg.seg_num;
    int thr_num=arg.thr_num;
    double segm=(arg.b-arg.a)/seg_num;
    double x1, x2;
    
    double *val=(double*)calloc(1, sizeof(double));
    double sum=0, buf;
    if (!val)
        handle_error("Cannot allocate memory\n");
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

void handle_error(char *errstr)
{
    int n=strlen(errstr);
    write(STDERR_FILENO, (void*)errstr, n);
    exit(EXIT_FAILURE);
    return;
}
