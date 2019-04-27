#include <stdio.h>
/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* a work is 4 bytes */
#define WSIZE 4

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

//Masks
#define USED 0x00000001
#define NO_FLAGS 0x00000000
#define PREV_FREE 0x00000002
#define EXTRA_FLAG 0x00000004
#define SIZE_MASK 0xFFFFFFF8
//#define NULL 0x00000000

//Header interactions
#define IS_USED(header) (((int)header&USED))
#define IS_PREV_BLOCK_FREE(header) (((int)header & PREV_FREE))
#define IS_EXTRA_FLAG_SET(header) (((int)header & EXTRA_FLAG))
#define BLOCK_SIZE(header) (((int)header & SIZE_MASK))
#define SET_FLAG(header, flags) ((int)header|(flags))
#define CLEAR_FLAG(header, flags) ((int)header&(~(flags)))
#define HEADER(size, flags) ((void*)(size|(flags)))

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
extern void mm_print(void);
extern int print_header(void* header, char flag);
extern int mm_init (void);
extern void *mm_malloc (size_t size);
extern void mm_free (void *ptr);
extern void *mm_realloc(void *ptr, size_t size);
