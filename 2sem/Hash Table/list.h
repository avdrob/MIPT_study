//{=======================================================================
//! @file list.h
//! @date 7:12:2013
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 1.0
//!
//! This library contains a few functions for working with
//! double linked list.
//}=======================================================================

#include "memloc.h"
#include <string.h>
#include <assert.h>

#define DATASIZE 32
#define LISTSIZE 300

typedef struct list_node_t
{
    struct list_node_t* prev;
    struct list_node_t* next;
    char data [DATASIZE];
    int counter;
} list_node_t;

typedef struct
{
    list_node_t* head;
    list_node_t* tail;
    int elem_num;
} list_t;

//{==========================================================================
//! Function new_list - creates list of LISTSIZE.
//!
//! @return     Pointer to the list.
//}==========================================================================
list_t* new_list ();

//{==========================================================================
//! Function add_node - add a node to the list.
//!
//! @param      list    Pointer to the list.
//! @param      data    The data new node should contain.
//!
//! @return     Pointer to the node.
//}===========================================================================
list_node_t* add_node (list_t* list, const char data [DATASIZE]);

//{==========================================================================
//! Function delete_node - delete list node.
//!
//! @param      list        Pointer to the list.
//! @param      list_node   The node should be deleted.
//!
//! @return     0 if ti's ok.
//!             1 if there's no such a list or such a node.
//}===========================================================================
int delete_node (list_t* list, list_node_t* list_node);

//{==========================================================================
//! Function delete_list - delete all the list.
//!
//! @param      list    Pointer to the pointer to the list.
//!
//! @return     0 if ti's ok.
//!             1 if the list isn't ok, so the function can't remove it.
//}===========================================================================
int delete_list (list_t** list);

//{==========================================================================
//! Function clear_list - delete all the list node;
//!                       leave only list head structure.
//!
//! @param      list    Pointer to the list.
//!
//! @return     0 if ti's ok.
//!             1 if something was wrong.
//}===========================================================================
int clear_list (list_t* list);

//{==========================================================================
//! Function list_size - returns number of list's nodes.
//!
//! @param      list    Pointer to the list.
//!
//! @return     Number of nodes.
//!             -1 if there's no such a list.
//}===========================================================================
int list_size (list_t* list);

//{==========================================================================
//! Function list_insert - insert a node at the particular position.
//!
//! @param      list        Pointer to the list.
//! @param      list_node   Pointer to the node new node should be
//!                         inserted near.
//! @param      data        The data new node should contain.
//! @param      mode        Indicates where the new list should be inserted:
//!                         before the list_node or after it.
//!
//! @return     0 if ti's ok.
//!             NULL if it's impossible to insert.
//}===========================================================================
list_node_t* list_insert (list_t* list, list_node_t* list_node, const char data [DATASIZE], const char* mode);

//{==========================================================================
//! Function list_search - searches for the node with particular data.
//!
//! @param      list    Pointer to the list.
//! @param      data    The data should be found.
//!
//! @return     Pointer to the node containing such a data.
//!             Null if there's no such a node.
//}===========================================================================
list_node_t* list_search (list_t* list, const data [DATASIZE]);

//{==========================================================================
//! Function list_ok - checks out if the list is ok.
//!
//! @param      list    Pointer to the list.
//!
//! @return     1 if it's ok;
//!             0 if it isn't.
//}===========================================================================
int list_ok (list_t* list);

//{==========================================================================
//! Function node_ok - checks out if the node is ok.
//!
//! @param      node    Pointer to the node.
//!
//! @return     1 if it's ok;
//!             0 if it isn't.
//}===========================================================================
int node_ok (list_node_t* list_node);

//{==========================================================================
//! Function list_dump - prints info about list.
//!
//! @param      list    Pointer to the list.
//!
//! @return     nothing to be returned.8
//}===========================================================================
void list_dump (list_t* list);

list_t* new_list ()
{
    list_t* list = (list_t*) calloc (1, sizeof (list_t));
    set_mem (LISTSIZE, sizeof (list_node_t));
    list->elem_num = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

list_node_t* add_node (list_t* list, const char data [DATASIZE])
{
    list_node_t* list_node = seize_mem (sizeof (list_node_t));

    if (!list_node)
    {
        set_mem (LISTSIZE, sizeof (list_node_t));
        list_node = seize_mem (sizeof (list_node_t));
    }

    list_node->counter = 1;
    //memset (list_node->data, '\0', DATASIZE);
    strcpy (list_node->data, data);

    if (!(list->elem_num))
    {
        list->head = list_node;
        list_node->prev = NULL;
    }
    else
    {
        list->tail->next = list_node;
        list_node->prev = list->tail;
        list_node->next = NULL;
    }

    list->tail = list_node;
    (list->elem_num)++;

    return list_node;
}

int delete_node (list_t* list, list_node_t* list_node)
{
    if ((!list_node) || (!list))
        return 1;

    if (list_node->next)
        list_node->next->prev = list_node->prev;
    if (list_node->prev)
        list_node->prev->next = list_node->next;

    if (list->head == list_node)
        list->head = list_node->next;
    if (list->tail == list_node)
        list->tail = list_node->prev;

    (list->elem_num)--;
    free_mem (list_node, sizeof (list_node));

    return (list_node->prev && list_node->next && 1);
}

int delete_list (list_t** list)
{
    if (!(list_ok (*list)))
        return 1;

    dispose_mem (sizeof (list_node_t));

    (*list)->elem_num = 0;
    (*list)->head = NULL;
    (*list)->tail = NULL;
    free (*list);
    (*list) = NULL;

    return *list;
}

int clear_list (list_t* list)
{
    int cnt = 0;
    list_node_t* list_node = list->head;

    for (cnt = 1; cnt <= list->elem_num; cnt++)
    {
        assert (cnt <= list->elem_num);
        free_mem (list_node, sizeof (list_node_t));
        list_node = list_node->next;
    }

    list->elem_num = 0;
    list->head = NULL;
    list->tail = NULL;

    return (list_node && 1);
}

int list_size (list_t* list)
{
    if (!list)
        return -1;
    else
        return list->elem_num;
}

list_node_t* list_insert (list_t* list, list_node_t* list_node, const char data [DATASIZE], const char* mode)
{
    list_node_t* new_node = seize_mem (sizeof (list_node_t));

    if (!new_node)
    {
        set_mem (LISTSIZE, sizeof (list_node_t));
        new_node = seize_mem (sizeof (list_node_t));
    }

    strcpy (new_node->data, data);
    (new_node->counter)++;
    (list->elem_num)++;

    if (!strcmp (mode, "b"))
    {
        if (list_node->prev)
            list_node->prev->next = new_node;

        new_node->prev = list_node->prev;
        new_node->next = list_node;
        list_node->prev = new_node;

        if (list->head == list_node)
            list->head = new_node;

        return new_node;
    }

    if (!strcmp (mode, "a"))
    {
        if (list_node->next)
            list_node->next->prev = new_node;

        new_node->prev = list_node;
        new_node->next = list_node->next;
        list_node->next = new_node;

        if (list_node == list->tail)
            list->tail = new_node;

        return new_node;
    }

    (new_node->counter)--;
    free_mem (new_node, sizeof (list_node_t));
    (list->elem_num)--;
    return NULL;
}

list_node_t* list_search (list_t* list, const data [DATASIZE])
{
    /*int cnt = 0, node_num = list_size (list);
    list_node_t* list_node = list->head;

    //for (cnt = 1; cnt <= list->elem_num; cnt++)
    for (cnt = 1; cnt <= node_num; cnt++)
    {
        //assert (cnt <= list->elem_num);
        //if (!strcmp (data, list_node->data))

        if (!mystrcmp (data, list_node->data))
            return list_node;

        list_node = list_node->next;
    }

    return NULL;*/
    if (!(list->head))
        return NULL;

    //printf ("%d\n", list_size (list));
    void* buf = adv_search ((char*)(list->head->data), data, list_size (list));
    //printf ("%x %x\n", buf, list->head);

    return buf;
}

int node_ok (list_node_t* list_node)
{
    return ((!list_node->prev && list_node->next && (list_node->next->prev == list_node)) ||
            (list_node->prev && !list_node->next && (list_node->prev->next == list_node)) ||
            (!list_node->prev && !list_node->next) ||
            (list_node->prev && list_node->next && (list_node->next->prev == list_node) &&
            (list_node->prev->next == list_node)));
}

int list_ok (list_t* list)
{
    if ((list->head && !(list->tail)) || (!(list->head) && list->tail))
        return 0;

    if (!list)
        return 0;

    list_node_t* list_node = list->head;
    int cnt = 0;
    int ok_flag = 1;

    for (cnt = 0; ; cnt++)
    {
        if (!(list_node) || (cnt > list->elem_num))
            break;

        if (!(node_ok (list_node)))
            ok_flag = 0;

        list_node = list_node->next;
    }

    return ((cnt == list->elem_num) && ok_flag);
}

void list_dump (list_t* list)
{
    if (!list)
    {
        printf ("\n#ERROR: list doesn't exist\n");
        return;
    }

    printf ("\n#########################################################################\n");
    printf ("#List [%x] is ", list);
    if (list_ok (list))
        printf ("ok                                                   #\n");
    else
        printf ("BAD!                                                 #\n");

    printf ("#%71s#\n", " ");
    printf ("#Head node:            [%7x]                                        #\n", list->head);
    printf ("#Tail node:            [%7x]                                        #\n", list->tail);
    printf ("#Number of elements: %10d                                         #\n", list->elem_num);
    printf ("#%71s#\n", " ");
    printf ("#########################################################################\n");


    list_node_t* list_node = list->head;
    int cnt = 0;

    for (cnt = 1; cnt <= list->elem_num; cnt++)
    {
        assert (cnt <= list->elem_num);

        printf ("#Node %5d [%7x]: %20s     [%7x]     [%7x]#\n", cnt, list_node, list_node->data, list_node->prev, list_node->next);
        list_node = list_node->next;
    }

    printf ("#########################################################################\n");

    return;
}
