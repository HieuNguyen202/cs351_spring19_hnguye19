/*
Test c syctax
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

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


//Return the size of the header
int print_header(void* header, char flag){
    printf("[%c%d|%c%c%c]", flag, BLOCK_SIZE(header), IS_EXTRA_FLAG_SET(header)?'1':'0', IS_PREV_BLOCK_FREE(header)?'1':'0', IS_USED(header)?'1':'0');
    return BLOCK_SIZE(header);
}

void test_HEADER(){
    for (uint i = 0; i < 1000; ++i) {
        printf("HEADER(%08X) = %d\n", i, HEADER(i, USED));
    }
}

void test_IS_USED(){
    for (uint i = 0; i < 1000; ++i) {
        printf("IS_USED(%08X) = %d\n", i, IS_USED(i));
    }
}
void test_IS_PREV_BLOCK_FREE(){
    for (uint i = 0; i < 1000; ++i) {
        printf("IS_PREV_BLOCK_FREE(%08X) = %d\n", i, IS_PREV_BLOCK_FREE(i));
    }
}

void test_SET_FLAG(){
    for (uint i = 0; i < 1000; ++i) {
        printf("SET_FLAG(%08X) = %d\n", i, SET_FLAG(i, USED));
    }
}

void test_CLEAR_FLAG(){
    for (uint i = 0; i < 1000; ++i) {
        printf("SET_FLAG(%08X) = %d\n", i, SET_FLAG(i, USED));
        printf("CLEAR_FLAG(%08X) = %d\n", i, CLEAR_FLAG(i, USED));
    }
}

void test_BLOCK_SIZE(){
    for (uint i = 0; i < 1000; ++i) {
        printf("BLOCK_SIZE(%08X) = %d\n", i, BLOCK_SIZE(i));
    }
}

int main(int argc, char **argv){
//    test_IS_USED();
//    test_IS_PREV_BLOCK_FREE();
//    test_BLOCK_SIZE();
//    test_SET_FLAG();
//    test_HEADER();
//    test_CLEAR_FLAG();
    void* header = (void*)9;
    print_header(header, 'r');
}

