//{=======================================================================
//! @file memloc.h
//! @date 7:12:2013 11:01
//! @author Andrey Drobyshev <immortalguardian1@gmail.com>
//! @version 1.0
//!
//! This library contains user's memory allocator functions.
//}=======================================================================

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdint.h>

#define PAGES_NUMBER 256

typedef struct
{
    uint32_t* FreeBlocksHead;
    uint32_t* MemBlocksArray;
    int blocks_num;
    int block_size;
} mem_page;

mem_page *User_Mem [PAGES_NUMBER] = {};

//{==========================================================================
//! Function set_mem - creates new user's memory block.
//!
//! @param      blocks_num      Number of blocks.
//! @param      block_size      BLock's size.
//!
//! @return     Nothing to be returned.
//!
//! @note	    The function has assertion checks.
//}===========================================================================
void set_mem (int blocks_num, int block_size);

//{==========================================================================
//! Function seize_mem - captures a memory block from user's memory.
//!
//! @param      page_num    User's mempry page which is needed.
//!
//! @return     Pointer to this block.
//}===========================================================================
void* seize_mem (int page_num);

//{==========================================================================
//! Function free_mem - returnes a block to the free blocks list.
//!
//! @param      ptr         The block should be released.
//! @param      page_num    User's mempry page which is needed.
//!
//! @return     Nothing to be returned.
//}===========================================================================
void free_mem (void* ptr, int page_num);

//{==========================================================================
//! Function dispose_mem - release all the user's memory.
//!
//! @param      page_num - user's mempry page which is needed.
//!
//! @return     Nothing to be returned.
//}===========================================================================
void dispose_mem (int page_num);

void set_mem (int blocks_num, int block_size)
{
    if (block_size < sizeof (uint8_t*))
        block_size = sizeof (uint8_t*);

    int cnt = 0, page_num = block_size;
    uint8_t* pointer = calloc (blocks_num, block_size);

    if (!(User_Mem [page_num]))
        User_Mem [page_num] = (mem_page*) calloc (1, sizeof (mem_page));

    for (cnt = 0; cnt <= blocks_num - 2; cnt++)
    {
        assert (cnt <= blocks_num - 2);
        uint8_t* temp = pointer + cnt * block_size;
        *((uint8_t**)temp) = temp + block_size;
    }

    if ((User_Mem [page_num])->MemBlocksArray)
        pointer [blocks_num - 1] = (User_Mem [page_num])->FreeBlocksHead;
    else
        pointer [blocks_num - 1] = NULL;

    (User_Mem [page_num])->FreeBlocksHead = (User_Mem [page_num])->MemBlocksArray = &(pointer [0]);
    (User_Mem [page_num])->blocks_num += blocks_num;
    (User_Mem [page_num])->block_size = block_size;

    return;
}

void* seize_mem (int page_num)
{
    if (!((User_Mem [page_num])->FreeBlocksHead))
        return NULL;

    void* ptr = (User_Mem [page_num])->FreeBlocksHead;
    (User_Mem [page_num])->FreeBlocksHead = *((User_Mem [page_num])->FreeBlocksHead);

    return ptr;
}

void free_mem (void* ptr, int page_num)
{
    if (!ptr)
        return;

    *((uint8_t**)ptr) = (User_Mem [page_num])->FreeBlocksHead;
    (User_Mem [page_num])->FreeBlocksHead = ptr;

    return;
}

void dispose_mem (int page_num)
{
    if ((User_Mem [page_num])->MemBlocksArray)
        free ((User_Mem [page_num])->MemBlocksArray);

    (User_Mem [page_num])->blocks_num = 0;
    (User_Mem [page_num])->block_size = 0;
    (User_Mem [page_num])->FreeBlocksHead = NULL;
    (User_Mem [page_num])->MemBlocksArray = NULL;

    return;
}
