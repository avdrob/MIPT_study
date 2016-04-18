#include <stdio.h>

#define BUF_SIZE 100

char* EXPR_PTR = NULL;

double Get_G (const char expr_str [BUF_SIZE]);
double Get_E ();
double Get_T ();
double Get_P ();
double Get_N ();
void syntax_error ();

double main (int argc, char** argv)
{
    char expr_str [BUF_SIZE] = {};

   printf ("%g\n", Get_G (argv [1]));

    return 0;
}

double Get_G (const char expr_str [BUF_SIZE])
{
    EXPR_PTR = expr_str;

    double res = Get_E ();

    if (*EXPR_PTR != '\0')
        syntax_error ();

    return res;
}

double Get_E ()
{
    double val1 = Get_T ();

    while (*EXPR_PTR == '+' || *EXPR_PTR == '-')
    {
        int oper = *EXPR_PTR;
        EXPR_PTR++;
        double val2 = Get_T ();

        switch (oper)
        {
            case '+':
            {
                val1 += val2;
                break;
            }

            case '-':
            {
                val1 -= val2;
                break;
            }
        }
    }

    return val1;
}

double Get_T ()
{
    double val1 = Get_P ();

    while (*EXPR_PTR == '*' || *EXPR_PTR == '/')
    {
        int oper = *EXPR_PTR;
        EXPR_PTR++;
        double val2 = Get_P ();

        switch (oper)
        {
            case '*':
            {
                val1 *= val2;
                break;
            }

            case '/':
            {
                val1 /= val2;
                break;
            }
        }
    }

    return val1;
}

double Get_P ()
{
    if (*EXPR_PTR == '(')
    {
        EXPR_PTR++;
        double val1 = Get_E ();

        if (*EXPR_PTR != ')')
            syntax_error ();

        EXPR_PTR++;

        return val1;
    }

    else
        return Get_N ();
}

double Get_N ()
{
    double num = 0;

    if (!('0' <= *EXPR_PTR && *EXPR_PTR <= '9'))
        syntax_error ();

    while ('0' <= *EXPR_PTR && *EXPR_PTR <= '9')
    {
        num = num * 10 + *EXPR_PTR - '0';
        EXPR_PTR++;
    }

    return num;
}

void syntax_error ()
{
    printf ("#SYNTAX ERROR\n");
    abort ();
}
