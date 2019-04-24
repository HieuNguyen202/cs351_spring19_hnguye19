/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 
 * mm_init - initialize the malloc package.
 * : Before calling mm_malloc, mm_realloc or mm_free, the application program (i.e., the
trace-driven driver program that you will use to evaluate your implementation) calls mm_init to perform
any necessary initializations, such as allocating the initial heap area. The return value should be -1 if
there was a problem in performing the initialization, 0 otherwise.
 */
int mm_init(void)
{
  return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 *     The mm_malloc routine returns a pointer to an allocated block payload of at least size
bytes. The entire allocated block should lie within the heap region and should not overlap with any other
allocated chunk.
We will comparing your implementation to the version of malloc supplied in the standard C library (libc).
Since the libc malloc always returns payload pointers that are aligned to 8 bytes, your malloc
implementation should do likewise and always return 8-byte aligned pointers.
 */
void *mm_malloc(size_t size)
{
  int newsize = ALIGN(size + SIZE_T_SIZE);
  void *p = mem_sbrk(newsize);
  if ((long)p == -1)    //sbrk failed
    return NULL;
  else {                //sbrk secceeded
    *(size_t *)p = size;        //header
    return (void *)((char *)p + SIZE_T_SIZE);     //The first address after the header
  }
}

/*
 * mm_free - Freeing a block does nothing.
 * The mm_free routine frees the block pointed to by ptr. It returns nothing. This routine is only
guaranteed to work when the passed pointer (ptr) was returned by an earlier call to mm_malloc or
mm_realloc and has not yet been freed.
 */
void mm_free(void *ptr)
{
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 * The mm_realloc routine returns a pointer to an allocated region of at least size bytes with
the following constraints.
○ if ptr is NULL, the call is equivalent to mm_malloc(size);
○ if size is equal to zero, the call is equivalent to mm_free(ptr);
○ if ptr is not NULL, it must have been returned by an earlier call to mm_malloc or mm_realloc.
The call to mm_realloc changes the size of the memory block pointed to by ptr (the old block) to
size bytes and returns the address of the new block. Notice that the address of the new block
might be the same as the old block, or it might be different, depending on your implementation,
the amount of internal fragmentation in the old block, and the size of the realloc request.
○ The contents of the new block are the same as those of the old ptr block, up to the minimum of
the old and new sizes. Everything else is uninitialized. For example, if the old block is 8 bytes
and the new block is 12 bytes, then the first 8 bytes of the new block are identical to the first 8
bytes of the old block and the last 4 bytes are uninitialized. Similarly, if the old block is 8 bytes
and the new block is 4 bytes, then the contents of the new block are identical to the first 4 bytes
of the old block.
 */
void *mm_realloc(void *ptr, size_t size)
{
  void *oldptr = ptr;
  void *newptr;
  size_t copySize;
    
  newptr = mm_malloc(size);
  if (newptr == NULL)
    return NULL;
  copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
  if (size < copySize)
    copySize = size;
  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);
  return newptr;
}

int mm_check(void){

}

//Naive Implementation
//Results for mm malloc:
//trace  valid  util     ops      secs  Kops
//0       yes   23%    5694  0.000060 94742
//1       yes   19%    5848  0.000055106327
//2       yes   30%    6648  0.000079 84259
//3       yes   40%    5380  0.000054100000
//4        no     -       -         -     -
//5        no     -       -         -     -
//6        no     -       -         -     -
//7       yes   55%   12000  0.000136 87912
//8       yes   51%   24000  0.000172139292
//9        no     -       -         -     -
//10        no     -       -         -     -
//Total            -       -         -     -