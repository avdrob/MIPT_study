//{=======================================================================
//! @file CPU_1.0.c
//! @date 3.12.2013 14:26
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 1.0
//!
//! This program emulates stack CPU.
//}=======================================================================

#include "../Stack/stckcount.h"
#include <strings.h>

unsigned const STACK_SIZE = 10;

//{==========================================================================
//! Function cpu_run - to read user's command and run it.
//!
//!
//! @return     1 if any operation is done.
//!             0 if none.
//}===========================================================================
int cpu_run (stack_t* stack);

//{==========================================================================
//! Function welcome_mes - just prints welcome message.
//!
//!
//! @return     Nothing to return.
//}===========================================================================
void welcome_mes ();

//{==========================================================================
//! Function help_mes - just prints the list of user's commands.
//!
//!
//! @return     Nothing to return.
//}===========================================================================
void help_mes ();

int main ()
{
    stack_t* cpu_stack = stack_new (STACK_SIZE);

    welcome_mes ();
    while (cpu_run (cpu_stack))
        ;
    elem_t buf_val = 0;
    stack_pop (cpu_stack, &buf_val);
    printf ("\n#%g\n", buf_val);

    return 0;
}

int cpu_run (stack_t* stack)
{
    char buf_str [] = "";
    double buf_val = 0;

    scanf ("%s", &buf_str);

    if (!(strcasecmp (buf_str, "help")))
    {
        help_mes ();
        return 1;
    }

    if (!(strcasecmp (buf_str, "push")))
    {
        scanf ("%lg", &buf_val);
        stack_push (stack, buf_val);

        return 1;
    }

    if (!(strcasecmp (buf_str, "dump")))
    {
        stack_dump (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "add")))
    {
        stack_add (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "sub")))
    {
        stack_sub (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "mul")))
    {
        stack_mul (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "div")))
    {
        stack_div (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "sin")))
    {
        stack_sin (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "cos")))
    {
        stack_cos (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "tan")))
    {
        stack_tan (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "sqrt")))
    {
        stack_sqrt (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "pow")))
    {
        stack_pow (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "exp")))
    {
        stack_exp (stack);

        return 1;
    }

    if (!(strcasecmp (buf_str, "halt")))
        return 0;
}

void welcome_mes ()
{
    printf ("###############################################################\n");
    printf ("#Welcome to Stack_CPU Ver1.0                                  #\n");
    printf ("#Copyright 2013 Andrey Drobyshev <immortalguardian1@gmail.com>#\n");
    printf ("#                                                             #\n");
    printf ("#For printing the list of commands: help                      #\n");
    printf ("#                                                             #\n");
    printf ("#Have fun!                                                    #\n");
    printf ("###############################################################\n\n");

    return ;
}

void help_mes ()
{
    printf ("\n");
    printf ("#Add\n");
    printf ("#Cos\n");
    printf ("#Div\n");
    printf ("#Dump\n");
    printf ("#End\n");
    printf ("#Exp\n");
    printf ("#Mul\n");
    printf ("#Pow\n");
    printf ("#Push <num>\n");
    printf ("#Sin\n");
    printf ("#Sqrt\n");
    printf ("#Sub\n");
    printf ("#Tan\n");

    return ;
}
