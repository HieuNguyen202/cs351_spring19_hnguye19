/*
Test c syctax
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "mm.h"

void test_HEADER(){
    for (uint i = 0; i < 1000; ++i) {
        printf("HEADER(%08X) = %d\n", i, MAKE_HEADER(i, USED));
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
        printf("BLOCK_SIZE_BYTES(%08X) = %d\n", i, BLOCK_SIZE_BYTES(i));
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
    mm_init();
    for (int i = 1; i < 17; ++i) {
        mm_malloc(i);
    }

    mm_print();

}

