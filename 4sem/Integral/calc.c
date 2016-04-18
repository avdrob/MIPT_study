#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

//#define SEGNUM 200000000

extern double calculate (double, double, double (*)(double), int, int);
extern void handle_error(char*);
double func(double);

char *USAGE="Wrong arguments\nUsage: ./calc a b threads_number segments_number\n";

int main(int argc, char *argv[])
{
    if (argc!=5)
        handle_error("Wrong arguments\n");
    char *errstr;
	double a=strtod(argv[1], (char**)&errstr);
    if (!((errstr[0]==' ') | (errstr[0]=='\n') | (errstr[0]=='\0')))
        handle_error(USAGE);
    double b=strtod(argv[2], (char**)&errstr);
    if (!((errstr[0]==' ') | (errstr[0]=='\n') | (errstr[0]=='\0')))
        handle_error(USAGE);
    long int thr_num=strtol(argv[3], (char**)&errstr, 10);
    if (!((errstr[0]==' ') | (errstr[0]=='\n') | (errstr[0]=='\0')))
        handle_error(USAGE);
    long int seg_num=strtol(argv[4], (char**)&errstr, 10);
    if (!((errstr[0]==' ') | (errstr[0]=='\n') | (errstr[0]=='\0')))
        handle_error(USAGE);
    
    if ((thr_num<=0) | (seg_num<=0))
        handle_error("Wrong arguments: threads_number and segments_number should be non-negative integers\n");
    	
	double I=calculate(a, b, func, thr_num, seg_num);
    printf("%lf\n", I);
        
	return 0;
}

double func(double x){
	return 4*x*x*x;
}
