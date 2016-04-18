#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef double elem_t;

const elem_t POISON = -666;

struct stack_t
{
    private:
        elem_t* data_;
        int counter_;
        int size_;

    public:
        stack_t ();
        ~stack_t ();
        void dump ();
        int empty ();
        int full ();
        int ok ();
        int push (elem_t);
        int pop (elem_t*);
        int top (elem_t*);
        int dup ();
};

stack_t :: stack_t ():
    data_ (new elem_t [1]),
    size_ (1),
    counter_ (0)
{
    //assert (data_);
}

stack_t :: ~stack_t ()
{
    for (int nullifier = 0; nullifier < size_; nullifier ++)
    data_[nullifier] = POISON;
    delete [] data_;
    data_ = NULL;
    size_ = counter_ = POISON;
}

int stack_t :: full ()
{
    return counter_ >= size_;
}

int stack_t :: empty ()
{
    return counter_ == 0;
}

int stack_t :: ok ()
{
    return (this && data_ && (size_ > 0) && (counter_ >= 0) && (counter_ <= size_));
}

int stack_t :: push (elem_t value)
{
    if (full ())
    {
        size_ *= 2;
        data_ = (elem_t*) realloc (data_, size_ * sizeof (elem_t));
    }

    data_[counter_++] = value;

    return 1;
}

int stack_t :: pop (elem_t* value)
{
    if (empty ())
        return 0;

    *value = data_[--counter_];
    data_[counter_] = POISON;
    return 1;
}

int stack_t :: top (elem_t* value)
{
    if (empty ())
        return 0;

    *value = data_[counter_ - 1];
    return 1;
}

void stack_t :: dump ()
{
    printf ("####################################\n");
    printf ("#Stack [%x] is", (int) this);
    if (ok ())
        printf (" ok\n");
    else
        printf (" BAD!\n");

    printf ("\n#Curent size = %d\n", counter_);
    printf (  "#Max    size = %d\n\n", size_);

    int printer = 0;
    while (printer < counter_)
        printf ("data [%d] = *%g\n", printer, data_[printer++]);
    while (printer < size_)
        printf ("data [%d] = %g\n", printer, data_[printer++]);

    printf ("####################################\n\n");

    return;
}

int stack_t :: dup ()
{
    if (full ())
        return 0;

    elem_t buf = 0;
    top (&buf);
    push (buf);

    return 1;
}
