//{=======================================================================
//! @file stack_cpu4.h
//! @date 11.11.2013 4:10
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 4.0
//!
//! This program contains functions for work with stack CPU.
//}=======================================================================

#include "../Stack/stckcount.h"
#include "../System/commands.h"
#include "../System/registers.h"
#include "../System/constants.h"
#include <strings.h>
#include <assert.h>
#include <stdio.h>

//{=========================================================================================================================
//! @def        ASSERT(cpu) - checks out if the cpu structure is ok. If it's not, prints dump.
//!
//! @param      cpu - adress of cpu structure.
//}=========================================================================================================================
#define ASSERT(cpu)                                                                                                        \
        {if (!(cpu_ok (cpu)))                                                                                              \
        {                                                                                                                  \
            printf ("####################################\n");                                                             \
            printf ("#All is gone bad\n");                                                                                 \
            printf ("#Program was unexpectedly aborted\n");                                                                \
            printf ("####################################\n\n");                                                           \
            cpu_dump (cpu);                                                                                                \
            abort ();                                                                                                      \
        }}


//{=========================================================================================================================
//! @def        EXE_ERROR - aborts program if something is wrong
//}=========================================================================================================================
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
    stack_t* call_stack;
    elem_t* mem;
    elem_t ax;
    elem_t bx;
    elem_t cx;
    elem_t dx;
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
//! @return     Pointer to the cpu.
//!
//! @note	    The function has assertion checks.
//}===========================================================================
cpu_t* cpu_new ();

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
elem_t cpu_run (cpu_t* const cpu, FILE** out_file);

//{==========================================================================
//! Function cpu_mov     Copies value to CPU register.
//!
//! @param      cpu      Pointer to cpu.
//! @param      arg1     Register code.
//! @param      arg2     A number or another register code.
//!
//! @return     1 if it's ok.
//}===========================================================================
int cpu_mov (cpu_t* cpu, elem_t arg1, elem_t arg2, const char* mode);

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
//! @param      val         A real number.
//!
//! @return     Integer value.
//}===========================================================================
int real_to_int (double val);

//{==========================================================================
//! Function in_reg    To read a number from keyboard and put it into
//!                    CPU register.
//!
//! @param      *cpu   Pointer to CPU structure.
//! @param      cnt    Memory counter current value.
//!
//! @return     0 if it's OK.
//!             -1 if there's no such a register.
//}===========================================================================
int in_reg (cpu_t* cpu, int cnt);

cpu_t* cpu_new ()
{
    cpu_t* cpu = (cpu_t*) calloc (1, sizeof (cpu_t));
    cpu->mem = (elem_t*) calloc (MEM_SIZE, sizeof (elem_t));
    cpu->ax = 0;
    cpu->bx = 0;
    cpu->cx = 0;
    cpu->dx = 0;
    cpu->stack = stack_new (STACK_SIZE);
    cpu->call_stack = stack_new (CALL_STACK_SIZE);

    ASSERT(cpu);

    return cpu;
}

int cpu_delete (cpu_t* cpu)
{
    ASSERT(cpu);

    cpu->ax = POISON;
    cpu->bx = POISON;
    cpu->cx = POISON;
    cpu->dx = POISON;

    stack_delete (cpu->stack);
    stack_delete (cpu->call_stack);

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
    cpu->call_stack = NULL;
    cpu->mem = NULL;

    return 0;
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

        case REG_BX:
        {
            stack_push (cpu->stack, cpu->bx);
            return 0;
        }

        case REG_CX:
        {
            stack_push (cpu->stack, cpu->cx);
            return 0;
        }

        case REG_DX:
        {
            stack_push (cpu->stack, cpu->dx);
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

        case REG_BX:
        {
            stack_pop (cpu->stack, &(cpu->bx));
            return 0;
        }

        case REG_CX:
        {
            stack_pop (cpu->stack, &(cpu->cx));
            return 0;
        }

        case REG_DX:
        {
            stack_pop (cpu->stack, &(cpu->dx));
            return 0;
        }
    }

    return 1;
}

int cpu_ok (const cpu_t* const cpu)
{
    return (cpu != NULL && stack_ok (cpu->stack) && cpu->mem != NULL && stack_ok (cpu->call_stack));
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
    printf ("\n#bx = %g\n", cpu->bx);
    printf ("\n#cx = %g\n", cpu->cx);
    printf ("\n#dx = %g\n", cpu->dx);

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

    if (!(cpu->call_stack))
        printf ("\n#ERROR: call stack doesn't exist\n");
    else
        stack_dump (cpu->call_stack);

    if (!(cpu->stack))
        printf ("\n#ERROR: stack doesn't exist\n");
    else
        stack_dump (cpu->stack);

    return;
}

elem_t cpu_run (cpu_t* const cpu, FILE** out_file)
{
    ASSERT (cpu);


    char halt_flag = 0;
    int cnt = -1;
    elem_t buf_val1 = 0, buf_val2 = 0;

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
                cpu_mov (cpu, cpu->mem [cnt + 1], cpu->mem [cnt + 2], "n");
                cnt += 2;

                break;
            }

            case CMD_MOV_REG:
            {
                cpu_mov (cpu, cpu->mem [cnt + 1], cpu->mem [cnt + 2], "r");
                cnt += 2;

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

            case CMD_JMP:
            {
                cnt = cpu->mem [cnt + 1];
                cnt--;

                break;
            }

            case CMD_JE:
            {
                stack_pop (cpu->stack, &buf_val1);
                stack_pop (cpu->stack, &buf_val2);

                if (buf_val1 == buf_val2)
                {
                    cnt = cpu->mem [cnt + 1];
                    cnt--;
                }
                else
                    cnt++;

                break;
            }

            case CMD_JNE:
            {
                stack_pop (cpu->stack, &buf_val1);
                stack_pop (cpu->stack, &buf_val2);

                if (buf_val1 != buf_val2)
                {
                    cnt = cpu->mem [cnt + 1];
                    cnt--;
                }
                else
                    cnt++;

                break;
            }

            case CMD_JA:
            {
                stack_pop (cpu->stack, &buf_val1);
                stack_pop (cpu->stack, &buf_val2);

                if (buf_val1 < buf_val2)
                {
                    cnt = cpu->mem [cnt + 1];
                    cnt--;
                }
                else
                    cnt++;

                break;
            }

            case CMD_JB:
            {
                stack_pop (cpu->stack, &buf_val1);
                stack_pop (cpu->stack, &buf_val2);

                if (buf_val1 > buf_val2)
                {
                    cnt = cpu->mem [cnt + 1];
                    cnt--;
                }
                else
                    cnt++;

                break;
            }

            case CMD_JAE:
            {
                stack_pop (cpu->stack, &buf_val1);
                stack_pop (cpu->stack, &buf_val2);

                if (buf_val1 <= buf_val2)
                {
                    cnt = cpu->mem [cnt + 1];
                    cnt--;
                }
                else
                    cnt++;

                break;
            }

            case CMD_JBE:
            {
                stack_pop (cpu->stack, &buf_val1);
                stack_pop (cpu->stack, &buf_val2);

                if (buf_val1 >= buf_val2)
                {
                    cnt = cpu->mem [cnt + 1];
                    cnt--;
                }
                else
                    cnt++;

                break;
            }

            case CMD_OUT:
            {
                stack_pop (cpu->stack, &buf_val1);
                fprintf (*out_file, "%g ", buf_val1);

                break;
            }

            case CMD_IN_REG:
            {
                in_reg (cpu, cnt);
                cnt++;

                break;
            }

            case CMD_CALL:
            {
                stack_push (cpu->call_stack, (cnt + 1));
                cnt = cpu->mem [cnt + 1];
                cnt--;
                break;
            }

            case CMD_RET:
            {
                stack_pop (cpu->call_stack, &buf_val1);
                cnt = buf_val1;
                break;
            }
        }

        if (halt_flag)
            break;
    }

    stack_pop (cpu->stack, &buf_val1);

    return  buf_val1;
}

int real_to_int (double val)
{
    return val;
}

int cpu_mov (cpu_t* cpu, elem_t arg1, elem_t arg2, const char* mode)
{
    elem_t* pointer1 = NULL;
    elem_t* pointer2 = NULL;

    if (mode == "n")
    {
        switch (real_to_int (arg1))
        {
            case REG_AX:
            {
                cpu->ax = arg2;
                return 0;
            }

            case REG_BX:
            {
                cpu->bx = arg2;
                return 0;
            }

            case REG_CX:
            {
                cpu->cx = arg2;
                return 0;
            }

            case REG_DX:
            {
                cpu->dx = arg2;
                return 0;
            }
        }
    }

    if (mode == "r")
    {
        switch (real_to_int (arg1))
        {
            case REG_AX:
            {
                pointer1 = &(cpu->ax);
                break;
            }

            case REG_BX:
            {
                pointer1 = &(cpu->bx);
                break;
            }

            case REG_CX:
            {
                pointer1 = &(cpu->cx);
                break;
            }

            case REG_DX:
            {
                pointer1 = &(cpu->dx);
                break;
            }
        }

        switch (real_to_int (arg2))
        {
            case REG_AX:
            {
                pointer2 = &(cpu->ax);
                break;
            }

            case REG_BX:
            {
                pointer2 = &(cpu->bx);
                break;
            }

            case REG_CX:
            {
                pointer2 = &(cpu->cx);
                break;
            }

            case REG_DX:
            {
                pointer2 = &(cpu->dx);
                break;
            }
        }

        *pointer1 = *pointer2;
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

int in_reg (cpu_t* cpu, int cnt)
{
    switch (real_to_int(cpu->mem [cnt + 1]))
    {
        case REG_AX:
        {
            scanf ("%lg", &(cpu->ax));
            return 0;
        }

        case REG_BX:
        {
            scanf ("%lg", &(cpu->bx));
            return 0;
        }

        case REG_CX:
        {
            scanf ("%lg", &(cpu->cx));
            return 0;
        }

        case REG_DX:
        {
            scanf ("%lg", &(cpu->dx));
            return 0;
        }
    }

    return -1;
}
