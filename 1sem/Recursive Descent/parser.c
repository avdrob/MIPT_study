#include <stdio.h>
#include <string.h>
#include <math.h>

#define BUF_SIZE 100

const double e = 2.718281828;
const double p = 3.14159;

char* EXPR_PTR = NULL;
double VARIABLES [26] = {};

double Get_G (FILE* input, const char expr_str [BUF_SIZE]);
double Get_Op ();
double Get_A ();
char Get_V ();
double Get_E ();
double Get_T ();
double Get_P ();
double Get_N ();
void syntax_error ();

int main ()
{
    char expr_str [BUF_SIZE] = {};
    FILE* input = fopen ("input.txt", "r");

    printf ("%g\n", Get_G (input, expr_str));

    return 0;
}

double Get_G (FILE* input, const char expr_str [BUF_SIZE])
{
    fscanf (input, "%s", expr_str);
    if (strcmp (expr_str, "<<"))
        syntax_error ();

    double res = 0;

    while (1)
    {
        char buf [BUF_SIZE] =  {};
        fscanf (input, "%s", buf);
        strcpy (expr_str, buf);
        int length = strlen (buf);

        while (buf [length - 1] != ';' && buf [length - 1] != EOF && strcmp (buf, "ret"))
        {
            fscanf (input, "%s", buf);
            length = strlen (buf);
            strcat (expr_str, buf);

            if (feof (input))
                syntax_error ();
        }

        if (!strcmp (expr_str, "ret"))
            break;

        EXPR_PTR = expr_str;
        res = Get_Op ();
    }

    if (strcmp (expr_str, "ret"))
        syntax_error ();

    fscanf (input, "%s", expr_str);
    EXPR_PTR = expr_str;
    res = Get_E ();

    fscanf (input, "%s", expr_str);

    if (strcmp (expr_str, ">>"))
        syntax_error ();

    fscanf (input, "%s", expr_str);
    if (!feof (input))
        syntax_error ();

    return res;
}

double Get_Op ()
{
    double val = 0;

    while (*EXPR_PTR != '\0')
    {
        val = Get_A ();

        if (*EXPR_PTR != ';')
            syntax_error ();

        EXPR_PTR++;
    }

    return val;
}

double Get_A ()
{
    double val = 0;

    if ('a' <= *EXPR_PTR && *EXPR_PTR <= 'z')
    {
        char var = Get_V ();

        if (*EXPR_PTR == '=')
        {
            EXPR_PTR++;
            val = Get_E ();
            VARIABLES [var] = val;
        }

        else
            syntax_error ();
    }

    return val;
}

char Get_V ()
{
    if ('a' <= *EXPR_PTR && *EXPR_PTR <= 'z')
        return *(EXPR_PTR++) - 'a';
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
        double val = Get_E ();

        if (*EXPR_PTR != ')')
            syntax_error ();

        EXPR_PTR++;

        return val;
    }

    if ('a' <= *EXPR_PTR && *EXPR_PTR <= 'z')
    {
        if ('a' <= *(EXPR_PTR + 1) && *(EXPR_PTR + 1) <= 'z')
        {
            if (*EXPR_PTR == 's')
            {
                if (*(EXPR_PTR + 1) == 'i' && *(EXPR_PTR + 2) == 'n' && *(EXPR_PTR + 3) == '(')
                {
                    EXPR_PTR += 3;
                    return sin (Get_E ());
                }
            }

            if (*EXPR_PTR == 'c')
            {
                if (*(EXPR_PTR + 1) == 'o' && *(EXPR_PTR + 2) == 's' && *(EXPR_PTR + 3) == '(')
                {
                    EXPR_PTR += 3;
                    return cos (Get_E ());
                }
            }

            if (*EXPR_PTR == 't')
            {
                if (*(EXPR_PTR + 1) == 'g' && *(EXPR_PTR + 2) == '(')
                {
                    EXPR_PTR += 2;
                    return tan (Get_E ());
                }
            }

            if (*EXPR_PTR == 'l')
            {
                if (*(EXPR_PTR + 1) == 'n' && *(EXPR_PTR + 2) == '(')
                {
                    EXPR_PTR += 2;
                    return log (Get_E ());
                }
            }
        }

        else
            return VARIABLES [Get_V ()];
    }

    else
        return Get_N ();
}

double Get_N ()
{
    double num = 0;
    double deg = 0;

    if (!('0' <= *EXPR_PTR && *EXPR_PTR <= '9'))
        syntax_error ();

    while ('0' <= *EXPR_PTR && *EXPR_PTR <= '9')
    {
        num = num * 10 + *EXPR_PTR - '0';
        EXPR_PTR++;
    }

    if (*EXPR_PTR == '.')
    {
        EXPR_PTR++;
        if (!('0' <= *EXPR_PTR && *EXPR_PTR <= '9'))
            syntax_error ();

        while ('0' <= *EXPR_PTR && *EXPR_PTR <= '9')
        {
            num = num * 10 + *EXPR_PTR - '0';
            EXPR_PTR++;
            deg++;
        }

        num = num / pow (10, deg);
    }

    return num;
}

void syntax_error ()
{
    printf ("#SYNTAX ERROR\n");
    abort ();
}
