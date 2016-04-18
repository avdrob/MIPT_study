//{=======================================================================
//! @file ASM_3.0.c
//! @date 8.11.2013 11:27
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 3.0
//!
//! This program translates asm program into stack CPU ver3 commands.
//}=======================================================================

#include "/home/andrey/Proga/Stack_CPU/System/arg_num.h"
#include "/home/andrey/Proga/Stack_CPU/System/commands.h"
#include "/home/andrey/Proga/Stack_CPU/System/registers.h"
#include "/home/andrey/Proga/Stack_CPU/System/constants.h"
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#define CHK_MEM         {                                                                                                  \
                            if ((*pointer) >= MEM_SIZE)                                                                    \
                            {                                                                                              \
                                printf ("\n######################################\n");                                     \
                                printf ("#Not enough memory\n");                                                           \
                                printf ("#Program was unexpectedly aborted\n");                                            \
                                printf ("######################################\n");                                       \
                                abort ();                                                                                  \
                            }                                                                                              \
                        }

#define INVALID_INPUT   {                                                                                                  \
                            printf ("######################################\n");                                           \
                            printf ("#Invalid input\n");                                                                   \
                            printf ("#Program was unexpectedly aborted\n");                                                \
                            printf ("######################################\n");                                           \
                            abort ();                                                                                      \
                        }





#define CHK_REG(name)   {                                                                                                  \
                            if (!(strcmp (buf_str, #name)))                                                                \
                            {                                                                                              \
                                *buf_val = REG_##name;                                                                     \
                                                                                                                           \
                                return;                                                                                    \
                            }                                                                                              \
                        }

//{==========================================================================
//! Function upcase - converts each letter of a string into upper case.
//!
//! @param      *str    Pointer to string.
//!
//! @return     0 if it's ok.
//}==========================================================================
int upcase (char* str);

//{==========================================================================
//! Function load_mem - fills in an array with CPU instructions.
//!
//! @param      **in_file   Pointer to input file structure.
//! @param      *mem        Pointer to memory array.
//! @param      *pointer    Pointer to the current element of memory array.
//!
//! @return     Nothing to be returned.
//!
//! @note	    The function has assertion checks.
//}==========================================================================
int load_mem (FILE** in_file, double* mem, unsigned* pointer);

//{==========================================================================
//! Function out - outputs memory array into output file.
//!
//! @param      **out_file  Pointer to output file structure.
//! @param      *mem        Pointer to memory array.
//! @param      pointer     Pointer to the current element of memory array.
//!
//! @return     0 if ut's ok.
//!
//! @note	    The function has assertion checks.
//}==========================================================================
int out (FILE** exec_file, double* mem, unsigned pointer);

//{==========================================================================
//! Function concat - merge 2 strings with addition "_" symbol between.
//!
//! @param      buf_str1   1st string.
//! @param      buf_str2   2nd string.
//!
//! @return     0 if it's ok.
//}==========================================================================
int concat (char buf_str1 [BUF_SIZE], char buf_str2 [BUF_SIZE]);

//{==========================================================================
//! Function def_reg - define register.
//!
//! @param      *buf_str   A string consists of register name.
//! @param      *buf_val   The variable where register code will be written.
//!
//! @return     Nothing o be returned.
//!
//! @note       The function has assertion checks.
//}==========================================================================
void def_reg (char* buf_str, double* buf_val);

//{==========================================================================
//! Function def_reg - define register.
//!
//! @param      *buf_str   A string consists of comand name.
//!
//! @return     Number of arguments.
//!
//! @note       The functio has assertion checks.
//}==========================================================================
int arg_num (const char* buf_str, FILE** in_file);

int main ()
{
    FILE* in_file = fopen (INPUT_FILE, "r");
    FILE* exec_file = fopen (EXECUTION_FILE, "w");

    assert (in_file != NULL);
    assert (exec_file != NULL);

    double* mem  = (double*) calloc (MEM_SIZE, sizeof (double));
    unsigned pointer = 0;

    load_mem (&in_file, mem, &pointer);
    fclose (in_file);

    out (&exec_file, mem, pointer);
    fclose (exec_file);

    return 0;
}

int upcase (char* str)
{
    unsigned cnt = 0;

    while (str [cnt] != '\0')
        str [cnt] = toupper (str [cnt++]);

    return 0;
}

int load_mem (FILE** in_file, double* mem, unsigned* pointer)
{
    double buf_val1 = 0, buf_val2 = 0;
    char buf_str1 [BUF_SIZE] = "", buf_str2 [BUF_SIZE] = "";

    while (1)
    {
        CHK_MEM;

        fscanf (*in_file, "%s", buf_str1);
        if (!(feof (*in_file)))
        {
            switch (arg_num (buf_str1, &in_file))
            {
                case 1:
                    #include "chk_cmd.h"

                case 2:
                    {
                        if (fscanf (*in_file, "%lg", &buf_val1))
                        {

                            if (!(feof (*in_file)))
                                concat (buf_str1, "NUM");
                            else
                                INVALID_INPUT;
                        }

                        else
                        {
                            concat (buf_str1, "REG");
                            fscanf (*in_file, "%s", buf_str2);
                            def_reg (buf_str2, &buf_val1);
                        }

                        #include "chk_2arg_cmd.h"
                    }

                case 3:
                    {
                        fscanf (*in_file, "%s", buf_str2);
                        def_reg (buf_str2, &buf_val1);

                        if (fscanf (*in_file, "%lg", &buf_val2))
                        {
                            if (!(feof (*in_file)))
                                concat (buf_str1, "NUM");
                            else
                                INVALID_INPUT;
                        }

                        else
                        {
                            concat (buf_str1, "REG");
                            fscanf (*in_file, "%s", buf_str2);
                            def_reg (buf_str2, &buf_val2);
                        }

                        #include "chk_3arg_cmd.h"
                    }
            }

            break;
        }

        else
            break;
    }

    return 0;
}

int concat (char buf_str1 [BUF_SIZE], char buf_str2 [BUF_SIZE])
{
    unsigned cnt1 = 0, cnt2 = 0;

    while (buf_str1 [cnt1] != '\0')
    {
        assert (cnt1 >= 0);
        assert (cnt1 <= BUF_SIZE - 1);

        cnt1++;
    }

    buf_str1 [cnt1++] = '_';

    while (buf_str2 [cnt2] != '\0')
    {
        assert (cnt1 >= 0);
        assert (cnt1 <= BUF_SIZE - 1);
        assert (cnt2 >= 0);
        assert (cnt2 <= BUF_SIZE - 1);

        buf_str1 [cnt1] = buf_str2 [cnt2];

        cnt1++;
        cnt2++;
    }

    buf_str1 [cnt1] = '\0';

    return 0;
}

int out (FILE** exec_file, double* mem, unsigned pointer)
{
    unsigned cnt = 0;

    for (cnt = 0; cnt <= pointer - 1; cnt++)
    {
        assert (cnt >= 0);
        assert (cnt <= pointer - 1);

        fprintf (*exec_file, "%g ", mem [cnt]);
    }

    return 0;
}

int arg_num (const char* buf_str, FILE** in_file)
{
    upcase (buf_str);

    #include "args.h"

    int NUM_COM = sizeof (arg_list) / sizeof (arg_list [0]);
    int cnt = 0;

    for (cnt = 0; cnt <= NUM_COM - 1; cnt++)
        if (!(strcmp (buf_str, (arg_list [cnt]).str)))
            return (arg_list [cnt]).num;

    printf ("######################################\n");
    printf ("#Incorrect command: %s\n", buf_str);
        INVALID_INPUT;
}

void def_reg (char* buf_str, double* buf_val)
{
    *buf_val = 0;
    upcase (buf_str);

    CHK_REG (AX);

    if (!(1<= *buf_val && *buf_val <= 4))
        INVALID_INPUT;

    return;
}
