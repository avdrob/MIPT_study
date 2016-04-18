//{=======================================================================
//! @file stack.h
//! @date 31.10.2013 12:07
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 1.1
//!
//! This library contains a few functions for working with stack.
//}=======================================================================

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define ASSERT_OK(stack)\
        {if (!(stack_ok (stack)))\
        {\
            printf ("####################################\n");\
            printf ("#All is gone bad\n");\
            printf ("#Program was unexpectedly aborted\n");\
            printf ("####################################\n\n");\
            stack_dump (stack);\
            abort ();\
        }}

typedef double elem_t;

const elem_t POISON = -666;

typedef struct
{
    elem_t* data;
    int counter;
    int size;
}
stack_t;

//{==========================================================================
//! Function stack_new - create stack of needed size.
//!
//! @param      st_size     Size of stack
//!
//! @return     Pointer to the stack.
//!
//! @note	    The function has assertion checks.
//}===========================================================================
stack_t* stack_new (unsigned st_size);

//{==========================================================================
//! Function stack_destruct - destroys the stack.
//!
//! @param      stack       Pointer to the stack we need to destroy
//!
//! @return     1 if it's been done.
//!
//! @note	    The function has assertion checks.
//}===========================================================================
int stack_delete (stack_t* stack);

//{==========================================================================
//! Function stack_dump - prints information about current stack state.
//!
//! @param      stack       Pointer to the stack.
//!
//! @return     Nothing to be returned.
//}===========================================================================
void stack_dump (const stack_t* stack);

//{==========================================================================
//! Function stack_empty - checks out if stack is empty or not.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 it's empty.
//!             0 if not.
//}===========================================================================
int stack_empty (const stack_t* const stack);

//{==========================================================================
//! Function stack_full - checks out if stack is full or not.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 it's full.
//!             0 if not.
//}===========================================================================
int stack_full (const stack_t* const stack);

//{==========================================================================
//! Function stack_ok - checks out if stack is ok.
//!
//! @param      stack   Pointer to the stack.
//!
//! @return     1 it's ok.
//!             0 if not.
//}===========================================================================
int stack_ok (const stack_t* const stack);

//{==========================================================================
//! Function stack_pop  Take an element from the stack's top.
//!                     Remove it from stack.
//!
//! @param      stack   Pointer to the stack.
//! @param      value   An adress of variable top element should be
//!                     written to.
//!
//! @return     1 it's operation's been done.
//!             0 if hasn't.
//!
//! @note       The function has assertio checks.
//}===========================================================================
int stack_pop (stack_t* const stack, elem_t* value);

//{==========================================================================
//! Function stack_push - put an element into the stack top.
//!
//! @param      stack   Pointer to the stack.
//! @param      value   A number should be written.
//!
//! @return     1 it's operation's been done.
//!
//! @note       The function has assertio checks.
//! @note       The function expands the stack if it's full.
//}===========================================================================
int stack_push (stack_t* const stack, elem_t value);

//{==========================================================================
//! Function stack_top - take an element from the stack's top.
//!
//! @param      stack   Pointer to the stack.
//! @param      value   An adress of variable top element should be
//!                     written to.
//!
//! @return     1 it's operation's been done.
//!             0 if hasn't.
//!
//! @note       The function has assertio checks.
//}===========================================================================
int stack_top (const stack_t* const stack, elem_t* value);

stack_t* stack_new (unsigned st_size)
{
    stack_t* stack = (stack_t*) calloc (1, sizeof(stack_t));
    stack->size = st_size;
    stack->counter = 0;
    stack->data = (elem_t*) calloc (st_size, sizeof(elem_t));

    ASSERT_OK(stack);

    return stack;
}

int stack_delete (stack_t* stack)
{
    ASSERT_OK(stack);

    unsigned nullifier = 0;

    for (nullifier = 0; nullifier <= stack->size - 1; nullifier++)
    {
        assert (nullifier >= 0);
        assert (nullifier <= stack->size - 1);

        stack->data [nullifier] = POISON;
    }

    free (stack->data);
    stack->data = NULL;
    stack->size = POISON;
    stack->counter = POISON;
    free (stack);

    return 1;
}

int stack_push (stack_t* stack, elem_t value)
{
    ASSERT_OK(stack);

    if (stack_full(stack))
    {
        stack->size *= 2;
        stack->data = realloc (stack->data, stack->size * sizeof(elem_t));
    }

    stack->data [stack->counter++] = value;

    ASSERT_OK(stack);

    return 1;
}

int stack_pop (stack_t* const stack, elem_t* value)
{
    ASSERT_OK(stack);

    if (stack_empty(stack))
        return 0;

    *value = stack->data [--stack->counter];
    stack->data [stack->counter] = POISON;

    ASSERT_OK(stack);

    return 1;

}

int stack_full (const stack_t* const stack)
{
    return (stack->counter >= stack->size);
}

int stack_empty (const stack_t* const stack)
{
    return (stack->counter <= 0);
}

int stack_ok (const stack_t* const stack)
{
    return ((stack != NULL) && (stack->data != NULL) && (stack->size > 0) &&
            (stack->counter >= 0) && (stack->counter <= stack->size));
}

void stack_dump (const stack_t* const stack)
{
    printf ("####################################\n");
    printf ("#Stack [%x] is", stack);
    if (stack_ok (stack))
        printf (" ok\n");
    else
        printf (" BAD!\n");

    printf ("\n#Curent size = %d\n", stack->counter);
    printf (  "#Max    size = %d\n\n", stack->size);

    int printer = 0;

    while (printer <= stack->counter - 1)
    {
        assert (printer <= stack->counter - 1);

        printf ("data [%d] = *%g\n", printer, stack->data [printer]);
        printer++;
    }

    for ( ; printer <= stack->size - 1; printer++)
    {
        assert (printer <= stack->size - 1);

        printf ("data [%d] = %g\n", printer, stack->data [printer]);
    }

    printf ("####################################\n\n");

    return;
}

int stack_top (const stack_t* const stack, elem_t* value)
{
    ASSERT_OK(stack);

    if (!(stack_empty (stack)))
    {
        *value = stack->data [stack->counter - 1];
        return 1;
    }
    else
        return 0;
}
