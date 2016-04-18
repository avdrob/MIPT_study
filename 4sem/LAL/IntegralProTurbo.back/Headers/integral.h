#ifndef INTEGRALPROTURBO_INTEGRAL
#define INTEGRALPROTURBO_INTEGRAL

#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct calcarg{
    double (*f)(double);
    long int thr_num;
    double a;
    double b;
    int num;
    long int seg_num;
} calcarg_t;

void *calc_routine(void*);
double calculate(double, double, double (*)(double), int, long int);
double func(double);
extern void handle_error(const char*, const char*);
extern void fatal_err(const char*);

#endif
