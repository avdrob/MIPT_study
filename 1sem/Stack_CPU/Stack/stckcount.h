//{=======================================================================
//! @file stckcount.h
//! @date 2.12.2013 12:37
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 1.0
//!
//! This library contains a few functions for stack computing.
//}=======================================================================

#include "stack.h"
#include <math.h>

//{==========================================================================
//! Function stack_add - takes 2 elements and put their sum into stack.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 if it's OK;
//!             0 if there're not enough elements.
//!
//! @note       The function has assertion checks.
//}===========================================================================
int stack_add (stack_t* const stack);

//{==========================================================================
//! Function stack_sub - takes 2 elements and put their diference into stack.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 if it's OK;
//!             0 if there're not enough elements.
//!
//! @note       The function has assertion checks.
//}===========================================================================
int stack_sub (stack_t* const stack);

//{==========================================================================
//! Function stack_mul - takes 2 elements and put their product into stack.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 if it's OK;
//!             0 if there're not enough elements.
//!
//! @note       The function has assertion checks.
//}===========================================================================
int stack_mul (stack_t* const stack);

//{==========================================================================
//! Function stack_div - takes 2 elements and put their quotient into stack.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 if it's OK;
//!             0 if there're not enough elements.
//!
//! @note       The function has assertion checks.
//}===========================================================================
int stack_div (stack_t* const stack);

//{==========================================================================
//! Function stack_exp - takes an element and put its exponent into stack.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 if it's OK;
//!             0 if there're not enough elements.
//!
//! @note       The function has assertion checks.
//}===========================================================================
int stack_exp (stack_t* const stack);

//{==========================================================================
//! Function stack_sin - takes an element and put its sine into stack.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 if it's OK;
//!             0 if there're not enough elements.
//!
//! @note       The function has assertion checks.
//}===========================================================================
int stack_sin (stack_t* const stack);

//{==========================================================================
//! Function stack_cos - takes an element and put its cosine into stack.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 if it's OK;
//!             0 if there're not enough elements.
//!
//! @note       The function has assertion checks.
//}===========================================================================
int stack_cos (stack_t* const stack);

//{==========================================================================
//! Function stack_tan - takes an element and put its tangent into stack.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 if it's OK;
//!             0 if there're not enough elements.
//!
//! @note       The function has assertion checks.
//}===========================================================================
int stack_sin (stack_t* const stack);

//{==========================================================================
//! Function stack_sqrt - takes an element and put square root
//! of it into stack.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 if it's OK;
//!             0 if there're not enough elements.
//!
//! @note       The function has assertion checks.
//}===========================================================================
int stack_sqrt (stack_t* const stack);

//{==========================================================================
//! Function stack_pow - takes 2 elements and build the second in degree
//! of the first.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 if it's OK;
//!             0 if there're not enough elements.
//!
//! @note       The function has assertion checks.
//}===========================================================================
int stack_pow (stack_t* const stack);

int stack_add (stack_t* const stack)
{
    ASSERT_OK(stack);

    if (stack->counter < 2)
        return 0;
    else
    {
        elem_t m = 0, n = 0;

        stack_pop (stack, &m);
        stack_pop (stack, &n);
        stack_push (stack, m + n);

        ASSERT_OK(stack);

        return 1;
    }
}

int stack_sub (stack_t* const stack)
{
    ASSERT_OK(stack);

    if (stack->counter < 2)
        return 0;
    else
    {
        elem_t m = 0, n = 0;

        stack_pop (stack, &m);
        stack_pop (stack, &n);
        stack_push (stack, n - m);

        ASSERT_OK(stack);

        return 1;
    }
}

int stack_mul (stack_t* const stack)
{
    ASSERT_OK(stack);

    if (stack->counter < 2)
        return 0;
    else
    {
        elem_t m = 0, n = 0;

        stack_pop (stack, &m);
        stack_pop (stack, &n);
        stack_push (stack, m * n);

        ASSERT_OK(stack);

        return 1;
    }
}

int stack_div (stack_t* const stack)
{
    ASSERT_OK(stack);

    if (stack->counter < 2)
        return 0;
    else
    {
        elem_t m = 0, n = 0;

        stack_pop (stack, &m);
        stack_pop (stack, &n);
        stack_push (stack, n / m);

        ASSERT_OK(stack);

        return 1;
    }
}

int stack_exp (stack_t* const stack)
{
    ASSERT_OK(stack);

    if (stack->counter < 1)
        return 0;
    else
    {
        elem_t m = 0;

        stack_pop (stack, &m);
        stack_push (stack, exp(m));

        ASSERT_OK(stack);

        return 1;
    }
}

int stack_sin (stack_t* const stack)
{
    ASSERT_OK(stack);

    if (stack->counter < 1)
        return 0;
    else
    {
        elem_t m = 0;

        stack_pop (stack, &m);
        stack_push (stack, sin(m));

        ASSERT_OK(stack);

        return 1;
    }
}

int stack_cos (stack_t* const stack)
{
    ASSERT_OK(stack);

    if (stack->counter < 1)
        return 0;
    else
    {
        elem_t m = 0;

        stack_pop (stack, &m);
        stack_push (stack, cos(m));

        ASSERT_OK(stack);

        return 1;
    }
}

int stack_tan (stack_t* const stack)
{
    ASSERT_OK(stack);

    if (stack->counter < 1)
        return 0;
    else
    {
        elem_t m = 0;

        stack_pop (stack, &m);
        stack_push (stack, tan(m));

        ASSERT_OK(stack);

        return 1;
    }
}

int stack_sqrt (stack_t* const stack)
{
    ASSERT_OK(stack);

    if (stack->counter < 1)
        return 0;
    else
    {
        elem_t m = 0;

        stack_pop (stack, &m);
        stack_push (stack, sqrt(m));

        ASSERT_OK(stack);

        return 1;
    }
}

int stack_pow (stack_t* const stack)
{
    ASSERT_OK(stack);

    if (stack->counter < 2)
        return 0;
    else
    {
        elem_t m = 0, n = 0;

        stack_pop (stack, &m);
        stack_pop (stack, &n);
        stack_push (stack, pow (n, m));

        ASSERT_OK(stack);

        return 1;
    }
}
