//{=======================================================================
//! @file CPU_3.0.c
//! @date 10.11.2013 21:10
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 3.0
//!
//! This program emulates stack processor.
//}=======================================================================

#include <stdio.h>
#include "stack_cpu3.h"

int main ()
{
    FILE* exec_file = fopen (EXECUTION_FILE, "r");
    FILE* out_file = fopen (OUTPUT_FILE, "w");
    cpu_t* cpu = cpu_new (STACK_SIZE);

    load_program (cpu, &exec_file);

    int i = 0;

    for (i = 1; i <= 10; i++)
    {
        cpu_mov (cpu, 1, i);
        fprintf (out_file, "%6d %10g\n", i, cpu_run (cpu));
    }

    fclose (out_file);
    return 0;
}
