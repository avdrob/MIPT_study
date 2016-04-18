//{=======================================================================
//! @file CPU_4.0.c
//! @date 11.11.2013 4:42
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 4.0
//!
//! This program executes emulates stack processor.
//}=======================================================================

#include <stdio.h>
#include "stack_cpu4.h"

int main ()
{
    FILE* exec_file = fopen (EXECUTION_FILE, "r");
    FILE* out_file = fopen (OUTPUT_FILE, "w");
    cpu_t* cpu = cpu_new ();

    load_program (cpu, &exec_file);
    cpu_run (cpu, &out_file);

    fclose (out_file);
    return 0;
}
