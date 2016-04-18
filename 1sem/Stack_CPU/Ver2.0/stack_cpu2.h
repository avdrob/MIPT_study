//{=======================================================================
//! @file stack_cpu2.h
//! @date 4.11.2013 10:37
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 2.0
//!
//! This library contains a few functions for working with stack CPU.
//}=======================================================================

#include "../Stack/stckcount.h"
#include <strings.h>

#define ASSERT(cpu)\
        {if (!(cpu_ok (cpu)))\
        {\
            printf ("####################################\n");\
            printf ("#All is gone bad\n");\
            printf ("#Program was unexpectedly aborted\n");\
            printf ("####################################\n\n");\
            cpu_dump (cpu);\
            abort ();\
        }}

typedef struct
{
    stack_t* stack;
    elem_t ax;
}
cpu_t;

//{==========================================================================
//! Function cpu_delete - destroys cpu structure.
//!
//! @param      cpu       Pointer to cpu we need to destroy.
//!
//! @return     1 if it's been done.
//!
//! @note	    The function has assertion checks.
//}==========================================================================
int cpu_delete (cpu_t* cpu);

//{==========================================================================
//! Function cpu_dump - prints information about current cpu state.
//!
//! @param      cpu       Pointer to cpu.
//!
//! @return     Nothing to be returned.
//}===========================================================================
void cpu_dump (const cpu_t* const cpu);

//{==========================================================================
//! Function cpu_new - create cpu structure of needed stack size.
//!
//! @param      st_size     Size of stack
//!
//! @return     Pointer to the cpu.
//!
//! @note	    The function has assertion checks.
//}===========================================================================
cpu_t* cpu_new (unsigned st_size);

//{==========================================================================
//! Function cpu_ok - checks out if cpu is ok.
//!
//! @param      cpu   Pointer to the stack.
//!
//! @return     1 it's ok.
//!             0 if not.
//}===========================================================================
int cpu_ok (const cpu_t* const cpu);

//{==========================================================================
//! Function cpu_run - reads user's asm program from text file and run it.
//!
//! @param      cpu      Pointer to cpu.
//! @param      file_in  Input file's name.
//!
//! @return     Result of program's work (stack top).
//!
//! @note	    The function has assertion checks.
//}===========================================================================
elem_t cpu_run (cpu_t* cpu, const char* file_in);

//{==========================================================================
//! Function cpu_mov_ax - moves value to ax CPU register.
//!
//! @param      cpu      Pointer to cpu.
//! @param      val      Value we need to move.
//!
//! @return     1 if it's ok.
//!
//! @note	    The function has assertion checks.
//}===========================================================================
int cpu_mov_ax (cpu_t* const cpu, elem_t val);

cpu_t* cpu_new (unsigned st_size)
{
    cpu_t* cpu = (cpu_t*) calloc (1, sizeof(cpu_t));
    cpu->ax = 0;
    cpu->stack = stack_new (st_size);

    ASSERT(cpu);

    return cpu;
}

int cpu_delete (cpu_t* cpu)
{
    ASSERT(cpu);

    cpu->ax = POISON;
    stack_delete (cpu->stack);
    free (cpu);
    cpu->stack = NULL;

    return 1;
}

int cpu_ok (const cpu_t* const cpu)
{
    return (cpu != NULL && stack_ok (cpu->stack));
}

void cpu_dump (const cpu_t* const cpu)
{
    printf ("####################################\n");
    printf ("#CPU [%x] is", cpu);
    if (cpu_ok (cpu))
        printf (" ok\n");
    else
        printf (" BAD!\n");

    printf ("\n#ax = %g\n", cpu->ax);
    if (!(cpu->stack))
        printf ("\n#ERROR: stack doesn't exist\n");
    else
        stack_dump (cpu->stack);

    return;
}

elem_t cpu_run (cpu_t* const cpu, const char* file_in)
{
    ASSERT(cpu);

    FILE* input_file = fopen (file_in, "r");
    char buf_str [10] = "";
    elem_t buf_val = 0;

    while (1)
    {
        fscanf (input_file, "%s", buf_str);

        if (!(strcasecmp (buf_str, "push")))
        {
            if (fscanf (input_file, "%lg", &buf_val))
                stack_push (cpu->stack, buf_val);
            else
            {
                fscanf (input_file, "%s", &buf_str);
                if (!(strcasecmp (buf_str, "ax")))
                    stack_push (cpu->stack, cpu->ax);
            }

            continue;
        }

        if (!(strcasecmp (buf_str, "dump")))
        {
            cpu_dump (cpu);

            continue;
        }

        if (!(strcasecmp (buf_str, "add")))
        {
            stack_add (cpu->stack);

            continue;
        }

        if (!(strcasecmp (buf_str, "sub")))
        {
            stack_sub (cpu->stack);

            continue;
        }

        if (!(strcasecmp (buf_str, "mul")))
        {
            stack_mul (cpu->stack);

            continue;
        }

        if (!(strcasecmp (buf_str, "div")))
        {
            stack_div (cpu->stack);

            continue;
        }

        if (!(strcasecmp (buf_str, "sin")))
        {
            stack_sin (cpu->stack);

            continue;
        }

        if (!(strcasecmp (buf_str, "cos")))
        {
            stack_cos (cpu->stack);

            continue;
        }

        if (!(strcasecmp (buf_str, "tan")))
        {
            stack_tan (cpu->stack);

            continue;
        }

        if (!(strcasecmp (buf_str, "exp")))
        {
            stack_exp (cpu->stack);

            continue;
        }

        if (!(strcasecmp (buf_str, "sqrt")))
        {
            stack_sqrt (cpu->stack);

            continue;
        }

        if (!(strcasecmp (buf_str, "pow")))
        {
            stack_pow (cpu->stack);

            continue;
        }

        if (!(strcasecmp (buf_str, "halt")))
            break;
    }

    stack_pop (cpu->stack, &buf_val);

    fclose (input_file);

    ASSERT(cpu);

    return buf_val;
}

int cpu_mov_ax (cpu_t* const cpu, elem_t val)
{
    ASSERT (cpu);

    cpu->ax = val;

    ASSERT(cpu);

    return 1;
}
