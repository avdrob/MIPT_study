//{=======================================================================
//! @file hash_tab.c
//! @date 7.12.2013 12:42
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 1.0
//!
//! This program puts text words into hash table of needed size.
//}=======================================================================

#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define BUFSIZE 20

//{==========================================================================
//! Function valid - checks out if the input data is valid.
//!
//! @param      hash_size   Input hash table size.
//! @param      func_num    Input hash function number (from 1 to 5)
//!
//! @note	    In case of invalid input the program will be aborted.
//}===========================================================================
int valid (char** argv);

//{==========================================================================
//! Function add_info - put the data into table.
//!
//! @param      hash_size   Input hash table size.
//! @param      hash_tab    Pointer to the hash table.
//! @param      input       Pointer to the input file.
//!
//! @return	    0 if it's ok.
//}===========================================================================
int add_info (int hash_size, hash_tab* table, FILE* input);

//{==========================================================================
//! Function set_word - remove all the excess symbols from the word;
//!                     convert it to lower case.
//!
//! @param      str     The word should be converted.
//! @param      input   Pointer to the input file.
//!
//! @return	    0 if it's ok (also in case EOF detected).
//}===========================================================================
int set_word (char* str, FILE* input);

//{==========================================================================
//! Function hash_funcN - calculates hash function of particular word.
//!                      Trere are 5 different functions.
//!                      N - function number.
//!
//! @param      str     The word.
//!
//! @return	    Hash function result.
//}===========================================================================
uint32_t hash_func (char* str, int num);

//{==========================================================================
//! Function lowcase - converts a word into lower case.
//!
//! @param      str     The word.
//!
//! @return	    0 if it's ok.
//}===========================================================================
int lowcase (char* str);

//{==========================================================================
//! Function output - writes down the hash table data allocation.
//!
//! @param      table       Pointer to the table.
//! @param      hash_szie   Hash table size.
//! @param      func_num    Nuber of hash function.
//!
//! @return	    Hash function result.
//}===========================================================================
void output (hash_tab* table, int hash_size);

//{===========================================================================
//! Function not_a_letter - checks out if symbol is letter.
//!
//! @param      symb - a symbol should be checked.
//!
//! @return     1 if it's a letter;
//!             0 if it's not.
//}===========================================================================
int not_a_letter (char symb);

uint32_t (*HASH_FUNC) (char*) = NULL;
FILE* OUTPUT_FILE = NULL;

int main (int argc, char** argv)
{
    int err_code = 0;
    if (err_code = valid (argv))
        return err_code;

    FILE* input = fopen ("input.txt", "r");
    int hash_size = atoi (argv [1]);
    hash_tab* table = new_hash (hash_size);

    strcpy (argv [2], "1");
    init (argv);

    while (1)
    {
        if (feof (input))
            break;

        add_info (hash_size, table, input);
    }

    output (table, hash_size);
    fclose (input);

    return 0;
}

int valid (char** argv)
{
    if (!(argv [1]) || !(argv [2]))
    {
        printf ("#Invalid input\n");
        printf ("#Enter hash table size and hash function number\n");
        return 1;
    }

    if (!(atoi (argv [1])))
    {
        printf ("#Invalid argument: %s\n", argv [1]);
        printf ("#Hash table size is a number\n");
        return 2;
    }

    if ((atoi (argv [2]) <= 0) || (6 <= (atoi (argv [2]))))
    {
        printf ("#Invalid argument: %s\n", argv [2]);
        printf ("#Hash function number should be in range [1..5]\n");
        return 3;
    }

    if (argv [3])
    {
        printf ("#Unknown argument: %s\n", argv [3]);
        return 4;
    }

    return 0;
}

int add_info (int hash_size, hash_tab* table, FILE* input)
{
    char str [BUFSIZE] = "";
    memset (str, '\0', BUFSIZE);

    fscanf (input, "%s", str);
    set_word (str, input);

    if (feof (input))
        return 0;

    if (str [0])
        add_data (table, (((uint32_t)(HASH_FUNC (str))) % ((uint32_t) hash_size)), str);

    return 0;
}


int set_word (char* str, FILE* input)
{
    if (feof (input))
        return 0;

    lowcase (str);

    while (not_a_letter (*str))
    {
        *str = '\0';
        str++;
    }

    str += strlen (str) - 1;

    while (not_a_letter (*str))
           str--;
    str++;
    *str = '\0';

    return 0;
}

uint32_t hash_func1 (char* str)
{
    return 0;
}

uint32_t hash_func2 (char* str)
{
    return str [0];
}

uint32_t hash_func3 (char* str)
{
    int sum = 0;
    while (*str != '\0')
    {
        sum += *str;
        str++;
    }

    return sum;
}

uint32_t hash_func4 (char* str)
{
    uint32_t sum = 0, length = strlen (str);

    while (*str != '\0')
        {
            sum += *str;
            str++;
        }

    return sum / length;
}

uint32_t hash_func5 (char* str)
{
    if (!(str [0]) || !(str [1]))
        return *str;

    uint32_t ror_sum = 0;

    while (*str != '\0')
    {
        ror_sum += (*str >> 1) | (*str << 31);
        str++;
    }

    return ror_sum;
}

int lowcase (char* str)
{
    unsigned cnt = 0;
    int length = strlen (str);

    for (cnt = 0; cnt <= length - 1; cnt++)
    {
        if ('A' <= *str && *str <= 'Z')
            *str += ('d' - 'D');
        if ('А' <= *str && *str <= 'Я')
            *str += ('б' - 'Б');
        str++;
    }

    return 0;
}

void output (hash_tab* table, int hash_size)
{
    int cnt = 0;

    for (cnt = 0; cnt <= hash_size - 1; cnt++)
    {
        list_t* list = &(*table [cnt]);

        int words_num = 0;

        if (list)
            words_num = list->elem_num;

        fprintf (OUTPUT_FILE, "%3d WORDS: ,", words_num);

        if (words_num)
        {
            list_node_t* list_node = list->head;
            int word_cnt = 0;

            for (word_cnt = 1; word_cnt <= list->elem_num; word_cnt++)
            {
                assert (list_node);
                fprintf (OUTPUT_FILE, "%s, ", list_node->data);
                list_node = list_node->next;
            }
        }
        fprintf (OUTPUT_FILE, "\n");
    }

    fclose (OUTPUT_FILE);

    return;
}

int init (char** argv)
{
    switch (atoi (argv [2]))
    {
        case 1:
            HASH_FUNC = hash_func1;
            OUTPUT_FILE = fopen ("func1.csv", "w");
            break;

        case 2:
            HASH_FUNC = hash_func2;
            OUTPUT_FILE = fopen ("func2.csv", "w");
            break;

        case 3:
            HASH_FUNC = hash_func3;
            OUTPUT_FILE = fopen ("func3.csv", "w");
            break;

        case 4:
            HASH_FUNC = hash_func4;
            OUTPUT_FILE = fopen ("func4.csv", "w");
            break;

        case 5:
            HASH_FUNC = hash_func5;
            OUTPUT_FILE = fopen ("func5.csv", "w");
            break;

        default:
            break;
    }

    return 0;
}

int not_a_letter (char symb)
{
    return (symb == '.' || symb == ',' || symb == '!' || symb == '?' || symb == '-' ||
           symb == '(' || symb == ')' || symb == '"' || ('0' <= symb && symb <= '9') ||
           symb == '\'' || symb == ':' || symb == '[' || symb == ']' || symb == '&' ||
           symb == '{' || symb == '}' || symb == '—');

}
