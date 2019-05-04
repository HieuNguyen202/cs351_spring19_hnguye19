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

/* 
 * mm_init - initialize the malloc package.
 * : Before calling mm_malloc, mm_realloc or mm_free, the application program (i.e., the
trace-driven driver program that you will use to evaluate your implementation) calls mm_init to perform
any necessary initializations, such as allocating the initial heap area. The return value should be -1 if
there was a problem in performing the initialization, 0 otherwise.
 */
header_t* heap_start;
header_t* prologue;
header_t* epilogue;
int mm_init(void)
{
    size_t init_words = 4;
    mem_init();
    if((heap_start = mem_sbrk(init_words*(WSIZE*8))) == (void*)-1) //get 4 words first
        return -1;
    prologue = heap_start + 1;
    heap_start[0] = MAKE_HEADER(0,NO_FLAGS);; //unused
    heap_start[1] = MAKE_HEADER(2*WSIZE,USED); //prologue
    heap_start[2] = MAKE_HEADER(2*WSIZE,USED); //prologue
    heap_start[3] = MAKE_HEADER(0,USED); //epilogue
    epilogue = heap_start + 3;
    return 0;
}

//Return the size of the header
void print_header(header_t * header, char flag){
    printf("[%p %c%d|%c%c%c]",header , flag, BLOCK_SIZE_BYTES(header)/WSIZE, IS_EXTRA_FLAG_SET(header)?'1':'0', IS_PREV_BLOCK_FREE(header)?'1':'0', IS_USED(header)?'1':'0');
}

void mm_print(void) {
    size_t size_words, size_bytes;
    for (header_t *header = heap_start; header <= epilogue; ++header) {
        if (header == heap_start) {
            print_header(header, 'u'); //unused block
            printf("\n");
        } else if (header == prologue || header == prologue + 1) {
            print_header(header, 'p'); //prologue
            printf("\n");
        } else if (BLOCK_SIZE_BYTES(header) == 0) {
            print_header(header, 'e'); //epilogue
            printf("\n");
        } else {
            print_header(header, 'r'); //regular header
            size_bytes = BLOCK_SIZE_BYTES(header);
            size_words = size_bytes/WSIZE;
            printf("[%d words]\n", (int)(size_words - 1));     //payload
            header += size_words - 1;
        }
    }
    printf("\n");
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
//    printf("malloc %d\n", size);

    size_t new_size_bytes = ALIGN(size + SIZE_T_SIZE);
    size_t block_size_bytes;
    int remaining;
    header_t *header = prologue;
    while(header < epilogue) { //Traverse all blocks
        if(!IS_USED(header)) { //If block is not used
            block_size_bytes = BLOCK_SIZE_BYTES(header);
            remaining = block_size_bytes - new_size_bytes;
            if(remaining >= 0){
                if(remaining < MIN_BLOCK_SIZE_WORDS * WSIZE){ //Get the whole free block
                    new_size_bytes = block_size_bytes;
                    *header = MAKE_HEADER(new_size_bytes, USED);
                    CLEAR_FLAG(NEXT_HEADER(header), PREV_FREE);
                    //*next block's prev free pointer = *current block's prev_free
                    //*prev block's next free = *current block's next_free
                } else{ //split and get the first block
                    *header = MAKE_HEADER(new_size_bytes, USED);    //header header for the first used block
                    *NEXT_HEADER(header) = MAKE_HEADER(remaining, NO_FLAGS);    //Make header for the remaining free block
                    *FOOTER(NEXT_HEADER(header)) = *NEXT_HEADER(header);        //Make footer for the remaining free block
                    //For the second block, update free pointers
                    //Copy prev free and next free from the original big block
                }
                //Update the next free block pointer of the previous free block
//                mm_print();
                return (void*)(header + 1);
            }
        }
        header += BLOCK_SIZE_WORDS(header); //header get the value of the next header
    }
//    printf("header %p, ep %p\n", header, epilogue);
    //Reach here mean no qualified free block found, need o sbrk more mem
    int sbrk_size_bytes = new_size_bytes;
    //If the last regular block is free, the new block's header is that free block's header
    if(IS_PREV_BLOCK_FREE(header)){ //If the last block is free but not big enough
        //Use it plus some more mem requested from the OS
        header = PREV_HEADER(header);
        sbrk_size_bytes -= BLOCK_SIZE_BYTES(header);
    } //else the new block's header has the address of the old epilogue block

    if(mem_sbrk(sbrk_size_bytes) == (void*)-1){
//        mm_print();
        return NULL; //sbrk failed
    }
    *header = MAKE_HEADER(new_size_bytes, USED); //allocate the new block

    epilogue = NEXT_HEADER(header); //update epilogue's address
    *epilogue = MAKE_HEADER(0,USED); //set epilogue value

    //if prev block was free
        //header become the header of that block
        //prev block's next free = new epilogue
        //epilogue's prev free = curr's prev
        //epilogue's next free = epilogue
    //if prev block was not free
        //epilogue is the current block
        //copy over free pointers to the new epilogue
        //prev's next free = epilogue
//    mm_print();
    return (void*)(header + 1); //return the address of the payload
  }

/*
 * mm_free - Freeing a block does nothing.
 * The mm_free routine frees the block pointed to by ptr. It returns nothing. This routine is only
guaranteed to work when the passed pointer (ptr) was returned by an earlier call to mm_malloc or
mm_realloc and has not yet been freed.
 */
void mm_free(void *ptr){
//    printf("free %p\n", ptr);

    header_t *header, *new_header, *next_header;
    header = ((header_t*)ptr) - 1;
    size_t s = BLOCK_SIZE_BYTES(header);
    CLEAR_FLAG(header, USED); //Make header unused
    //If prev block is free, combine
    new_header = header;
//    mm_print();
    while(IS_PREV_BLOCK_FREE(new_header)){
        new_header = PREV_HEADER(new_header);
        s += BLOCK_SIZE_BYTES(new_header);
    }

    //If next blocks are free, also combine
    next_header = NEXT_HEADER(header);
    while(!IS_USED(next_header)){
        s += BLOCK_SIZE_BYTES(next_header);
        next_header = NEXT_HEADER(next_header);
    }
    *new_header = MAKE_HEADER(s, NO_FLAGS); //Create new free header
    *FOOTER(new_header) = *new_header; //Make footer
    SET_FLAG(NEXT_HEADER(new_header), PREV_FREE); //Set PREV_FREE flag for the next used block
    //last free header = prev(next_header)
    //next free block's prev free = new_header
    //prev free block's next free = new_header
    //curr head's next free = next_header's next free
    //curr head's prev free = stays the same
//    mm_print();
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
    return 0;
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