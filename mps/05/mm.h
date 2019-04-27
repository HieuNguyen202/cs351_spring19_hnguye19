#include <stdio.h>
typedef unsigned int header_t;
/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* a word is 4 bytes */
#define WSIZE 4
#define MIN_BLOCK_SIZE_WORDS 2

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE 4

//Masks
#define USED 0x00000001
#define NO_FLAGS 0x00000000
#define PREV_FREE 0x00000002
#define EXTRA_FLAG 0x00000004
#define SIZE_MASK 0xFFFFFFF8
//#define NULL 0x00000000

//Header interactions
#define IS_USED(header) ((*header&USED))
#define IS_PREV_BLOCK_FREE(header) ((*header & PREV_FREE))
#define IS_EXTRA_FLAG_SET(header) ((*header & EXTRA_FLAG))
#define BLOCK_SIZE_BYTES(header) ((*header & SIZE_MASK))
#define BLOCK_SIZE_WORDS(header) (((*header & SIZE_MASK)/WSIZE))
#define SET_FLAG(header, flags) (*header = *header | (flags))
#define CLEAR_FLAG(header, flags) (*header = *header & (~(flags)))
#define MAKE_HEADER(size, flags) ((header_t)(size|(flags)))
#define NEXT_HEADER(header) (header + BLOCK_SIZE_WORDS(header))
#define PREV_FOOTER(header) (header - 1)
#define PREV_HEADER(header) (header - BLOCK_SIZE_WORDS(PREV_FOOTER(header)))
#define FOOTER(header) (NEXT_HEADER(header) - 1)

//#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

extern void mm_print(void);
extern void print_header(header_t *, char flag);
extern int mm_init (void);
extern void *mm_malloc (size_t size);
extern void mm_free (void *ptr);
extern void *mm_realloc(void *ptr, size_t size);
