//{=======================================================================
//! @file stack_cpu3.h
//! @date 4.11.2013 10:37
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 3.0
//!
//! This library contains a few functions for working with stack CPU.
//}=======================================================================

#include "../Stack/stckcount.h"
#include "../System/commands.h"
#include "../System/registers.h"
#include "../System/constants.h"
#include <strings.h>
#include <assert.h>
#include <stdio.h>

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

#define EXE_ERROR       {                                                                                                  \
                            printf ("######################################\n");                                           \
                            printf ("#Invalid input\n");                                                                   \
                            printf ("#Program was unexpectedly aborted\n");                                                \
                            printf ("######################################\n");                                           \
                            abort ();                                                                                      \
                        }


typedef struct
{
    stack_t* stack;
    elem_t* mem;
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
//! @param      cpu             Pointer to cpu.
//! @param      **exec_file     Input file's name.
//!
//! @return     Result of program's work (stack top).
//!
//! @note	    The function has assertion checks.
//}===========================================================================
elem_t cpu_run (cpu_t* const cpu);

//{==========================================================================
//! Function cpu_mov     Copies value to CPU register.
//!
//! @param      cpu      Pointer to cpu.
//! @param      arg1     Register code.
//! @param      arg2     A number or another register code.
//!
//! @return     1 if it's ok.
//}===========================================================================
int cpu_mov (cpu_t* cpu, elem_t arg1, elem_t arg2);

//{==========================================================================
//! Function push_reg    Pushes a value from register into stack.
//!
//! @param      cpu         Pointer to cpu.
//! @param      reg_code    Register code.
//!
//! @return     1 if it's ok.
//}===========================================================================
int cpu_push_reg (cpu_t* cpu, int reg_code);

//{==========================================================================
//! Function pop_reg        Moves a value from stack into register.
//!
//! @param      cpu         Pointer to cpu.
//! @param      reg_code    Register code.
//!
//! @return     1 if it's ok.
//}===========================================================================
int cpu_pop_reg (cpu_t* cpu, int reg_code);

//{==========================================================================
//! Function real_to_int    This magic function receives a real number
//!                         and gives it back just like it's integer.
//!
//! @param      cpu         Pointer to cpu.
//! @param      reg_code    Register code.
//!
//! @return     Integer value.
//}===========================================================================
int real_to_int (double val);

cpu_t* cpu_new (unsigned st_size)
{
    cpu_t* cpu = (cpu_t*) calloc (1, sizeof (cpu_t));
    cpu->mem = (elem_t*) calloc (MEM_SIZE, sizeof (elem_t));
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

    unsigned punisher = 0;

    for (punisher = 0; punisher <= MEM_SIZE - 1; punisher ++)
    {
        assert (punisher >= 0);
        assert (punisher <= MEM_SIZE - 1);

        cpu->mem [punisher] = POISON;
    }
    free (cpu->mem);
    free (cpu);
    cpu->stack = NULL;
    cpu->mem = NULL;

    return 1;
}

int cpu_push_reg (cpu_t* cpu, int reg_code)
{
    switch (reg_code)
    {
        case REG_AX:
        {
            stack_push (cpu->stack, cpu->ax);
            return 0;
        }
    }

    return 1;
}

int cpu_pop_reg (cpu_t* cpu, int reg_code)
{
    switch (reg_code)
    {
        case REG_AX:
        {
            stack_pop (cpu->stack, &(cpu->ax));
            return 0;
        }
    }

    return 1;
}

int cpu_ok (const cpu_t* const cpu)
{
    return (cpu != NULL && stack_ok (cpu->stack) && cpu->mem != NULL);
}

void cpu_dump (const cpu_t* const cpu)
{
    printf ("\n####################################\n");
    printf ("#CPU [%x] is", cpu);
    if (cpu_ok (cpu))
        printf (" ok\n");
    else
        printf (" BAD!\n");

    printf ("\n#ax = %g\n", cpu->ax);

    if (!(cpu->mem))
        printf ("\n#ERROR: memory blocks are damaged");
    else
    {
        printf ("####################################\n");
        printf ("#Memory [%x] map\n\n", cpu->mem);

        unsigned mem_prnt = 0;

        for (mem_prnt = 0; mem_prnt <= MEM_SIZE - 1; mem_prnt++)
        {
            assert (mem_prnt >= 0);
            assert (mem_prnt <= MEM_SIZE - 1);

            printf ("%4x    %g\n", mem_prnt, cpu->mem [mem_prnt]);
        }
    }

    if (!(cpu->stack))
        printf ("\n#ERROR: stack doesn't exist\n");
    else
        stack_dump (cpu->stack);

    return;
}

elem_t cpu_run (cpu_t* const cpu)
{
    ASSERT (cpu);

    char halt_flag = 0;
    int cnt = -1;

    while (1)
    {
        cnt++;

        switch (real_to_int (cpu->mem [cnt]))
        {
            case CMD_ERROR:
                EXE_ERROR;

            case CMD_HALT:
            {
                halt_flag = 1;
                break;
            }

            case CMD_PUSH_NUM:
            {
                stack_push (cpu->stack, cpu->mem [++cnt]);
                break;
            }

            case CMD_PUSH_REG:
            {
                cpu_push_reg (cpu, cpu->mem [++cnt]);
                break;
            }

            case CMD_POP_REG:
            {
                cpu_pop_reg (cpu, cpu->mem [++cnt]);
                break;
            }

            case CMD_MOV_NUM:
            {

                cpu_mov (cpu, cpu->mem [cnt++], cpu->mem [++cnt + 1]);
                break;
            }

            case CMD_ADD:
            {
                stack_add (cpu->stack);
                break;
            }

            case CMD_SUB:
            {
                stack_sub (cpu->stack);
                break;
            }

            case CMD_MUL:
            {
                stack_mul (cpu->stack);
                break;
            }

            case CMD_DIV:
            {
                stack_div (cpu->stack);
                break;
            }

            case CMD_SIN:
            {
                stack_sin (cpu->stack);
                break;
            }

            case CMD_COS:
            {
                stack_cos (cpu->stack);
                break;
            }

            case CMD_TAN:
            {
                stack_tan (cpu->stack);
                break;
            }

            case CMD_SQRT:
            {
                stack_sqrt (cpu->stack);
                break;
            }

            case CMD_EXP:
            {
                stack_exp (cpu->stack);
                break;
            }

            case CMD_POW:
            {
                stack_pow (cpu->stack);
                break;
            }

            case CMD_DUMP:
            {
                cpu_dump (cpu);
                break;
            }

            case CMD_NOP:
                break;
        }

        if (halt_flag)
            break;
    }

    elem_t buf_val = 0;

    stack_pop (cpu->stack, &buf_val);

    return  buf_val;
}

int real_to_int (double val)
{
    return val;
}

int cpu_mov (cpu_t* cpu, elem_t arg1, elem_t arg2)
{
    switch (real_to_int (arg1))
    {
        case REG_AX:
        {
            cpu->ax = arg2;
            return 0;
        }
    }

    return 1;
}

int load_program (cpu_t* cpu, FILE** exec_file)
{
    unsigned cnt = 0;
    elem_t buf_val = 0;

    while (1)
    {
        fscanf (*exec_file, "%lg", &buf_val);
        if (!(feof (*exec_file)))
            cpu->mem [cnt++] = buf_val;

        else
            break;
    }

    fclose (*exec_file);

    return 0;
}
