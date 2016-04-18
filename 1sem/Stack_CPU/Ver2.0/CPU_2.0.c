//{=======================================================================
//! @file CPU_2.0.c
//! @date 4.11.2013 10:39
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 2.0
//!
//! This program emulates work of stack CPU.
//}=======================================================================

#include "stack_cpu2.h"

unsigned const STACK_SIZE = 10;

//{==========================================================================
//! Function output - prints an argument and the corresponding
//!                   result of program work.
//!
//! @param      x       An argument.
//! @param      y       The result.
//!
//! @return     Nothing to return.
//!//}===========================================================================
void output (elem_t x, elem_t y);

int main ()
{
    cpu_t* cpu = cpu_new (STACK_SIZE);
    elem_t x = 0, y = 0;

    for (x = -10; x <= 10; x++)
    {
        cpu_mov_ax (cpu, x);
        y = cpu_run (cpu, "../program.in");
        output (x, y);
    }

    return 0;
}

void output (elem_t x, elem_t y)
{
    printf ("#x = %10.5g     y = %10.5g\n", x, y);

    return;
}
