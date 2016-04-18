#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define SEGNUM 20000000

int main()
{
    FILE *out1=fopen("./out1", "r"), *out2=fopen("./out2", "r");
    double buf1, buf2, sum1=0, sum2=0;
    int i;
    for (i=0; i<SEGNUM; i++){
        fscanf(out1, "%lf\n", &buf1);
        sum1+=buf1;
        fscanf(out2, "%lf\n", &buf2);
        sum2+=buf2;
        if (abs(buf1-buf2)>DBL_EPSILON){
            printf("ACHTUNG!!!1\n");
            break;
        }
    }
    printf("%lf %lf\n", sum1, sum2);
    fclose(out1);
    fclose(out2);
    
	return 0;
}
