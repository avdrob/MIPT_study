//{=======================================================================
//! @file hash.h
//! @date 7.12.2013 12:17
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 1.0
//!
//! This library contains a few functions for working with hash table.
//}=======================================================================

#include "list.h"

int HASH_TABLE_SIZE = 0;

typedef list_t** hash_tab;

//{==========================================================================
//! Function new_hash - create table of needed size.
//!
//! @param      size    Size of table.
//!
//! @return     Pointer to the table.
//}===========================================================================
hash_tab* new_hash (int size);

//{==========================================================================
//! Function hash_search - searches for particular data in particular list.
//!
//! @param          table       Pointer to the hash table.
//! @param          list_num    Number of list in the table.
//! @param          data        The data that should be found.
//!
//! @return     Pointer to the list node.
//!             NULL if there's no list with such a data.
//}===========================================================================
list_node_t* hash_search (hash_tab* table, int list_num, const char data [DATASIZE]);

//{==========================================================================
//! Function delete_hash - removes hash table.
//!
//! @param          table       Pointer to the pointer to the table.
//!
//! @return     0 if its ok;
//!             1 if there's no such a table or it's impossible to remove it.
//}===========================================================================
int delete_hash (hash_tab** table);

//{==========================================================================
//! Function hash_sok - checks out if the table is ok.
//!
//! @param          table       Pointer to the hash table.
//!
//! @return     1 if it's ok;
//!             0 if it isn't.
//}===========================================================================
int hash_ok (hash_tab* table);

//{==========================================================================
//! Function hash_dump - prints info about the table.
//!
//! @param          table       Pointer to the hash table.
//!
//! @return         Nothing to be returned.
//}===========================================================================
void hash_dump (hash_tab* table);

//{==========================================================================
//! Function add_data - adds particular data to the particular list.
//!
//! @param          table       Pointer to the hash table.
//! @param          list_num    Number of list in the table.
//! @param          data        The data that should be found.
//!
//! @return     Pointer to the list node.
//!             -1 if there's no such a table.
//}===========================================================================
list_node_t* add_data (hash_tab* table, int list_num, const char data [DATASIZE]);

hash_tab* new_hash (int size)
{
    HASH_TABLE_SIZE = size;
    hash_tab* table = (hash_tab*) calloc (HASH_TABLE_SIZE, sizeof (list_t*));

    return table;
}

list_node_t* hash_search (hash_tab* table, int list_num, const char data [DATASIZE])
{
    if (list_num < 0 || HASH_TABLE_SIZE <= list_num)
        return NULL;

    else
        return list_search (&(*table [list_num]), data);
}


int delete_hash (hash_tab** table)
{
    if (!(*(table)))
        return 1;

    int cnt = 0;

    for (cnt = 0; cnt <= HASH_TABLE_SIZE - 1; cnt++)
    {
        assert (cnt <= HASH_TABLE_SIZE - 1);
        list_t* list = &(*table [cnt]);
        if (list)
            delete_list (&list);
    }

    *table = NULL;

    return (*table && 1);
}

int hash_ok (hash_tab* table)
{
    if (!table)
        return 0;

    int cnt = 0;
    int ok_flag = 1;

    for (cnt = 0; cnt <= HASH_TABLE_SIZE - 1; cnt++)
    {
        assert (cnt <= HASH_TABLE_SIZE - 1);
        if ((table [cnt]))
        {
            if (!(list_ok ((table [cnt]))))
            {
                ok_flag = 0;
                break;
            }
        }
    }

    return ok_flag;
}

void hash_dump (hash_tab* table)
{
    if (!table)
    {
        printf ("\n##################################\n");
        printf ("#ERROR: hash table doesn't exist #\n");
        printf ("##################################\n");
        return;
    }

    printf ("\n#########################################################################\n");
    printf ("#Hash table [%x] is ", table);
    if (hash_ok (table))
        printf ("ok                                             #\n");
    else
        printf ("BAD!                                           #\n");

    printf ("#%71s#\n", " ");
    printf ("#Number of lists: %7d                                               #\n", HASH_TABLE_SIZE);

    printf ("#########################################################################\n");

    int cnt = 0;

    for (cnt = 0; cnt <= HASH_TABLE_SIZE - 1; cnt++)
    {
        assert (cnt <= HASH_TABLE_SIZE - 1);

        printf ("#List %5d [%7x] ", cnt, (table [cnt]));

        if (!(table [cnt]))
            printf ("is clear\n");

        else
        {
            if (list_ok ((table) [cnt]))
                printf ("is ok: ");
            else
                printf ("is BAD! :");

            int printer = 1;
            list_node_t* list_node = &((*table [cnt])->head);
            list_t* list = &(*table [cnt]);

            for (printer = 1; printer <= list->elem_num; printer++)
            {
                assert (printer <= list->elem_num);
                printf ("%s ", list_node->data);
                list_node = list_node->next;
            }
            printf("\n");
        }
    }
    printf ("#########################################################################\n");
    return;
}

list_node_t* add_data (hash_tab* table, int list_num, const char data [DATASIZE])
{
    if (!table)
        return NULL;

    list_node_t* list_node = NULL;

    if (!(table [list_num]))
        (table [list_num]) = new_list ();

    if (list_node = list_search ((table [list_num]), data))
        (list_node->counter)++;
    else
        list_node = add_node ((table [list_num]), data);

    return list_node;
}
