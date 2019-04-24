/*
Test c syctax
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define IS_USED(header) ((header&0x00000001))
#define IS_PREV_BLOCK_FREE(header) ((header&0x00000002))
#define BLOCK_SIZE(header) ((header&0xFFFFFFF8))

#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

void test_IS_USED(){
    printf("IS_USED(0x0003) = %d\n", IS_USED(0x0003));
    printf("IS_USED(0x0004) = %d\n", IS_USED(0x0004));
    printf("IS_USED(0x0005) = %d\n", IS_USED(0x0005));
    printf("IS_USED(0x0006) = %d\n", IS_USED(0x0006));
    printf("IS_USED(0x0007) = %d\n", IS_USED(0x0007));
    printf("IS_USED(0x0008) = %d\n", IS_USED(0x0008));
    printf("IS_USED(0x0009) = %d\n", IS_USED(0x0009));
}
void test_IS_PREV_BLOCK_FREE(){
    printf("IS_PREV_BLOCK_FREE(0x0003) = %d\n", IS_PREV_BLOCK_FREE(0x0003));
    printf("IS_PREV_BLOCK_FREE(0x0004) = %d\n", IS_PREV_BLOCK_FREE(0x0004));
    printf("IS_PREV_BLOCK_FREE(0x0005) = %d\n", IS_PREV_BLOCK_FREE(0x0005));
    printf("IS_PREV_BLOCK_FREE(0x0006) = %d\n", IS_PREV_BLOCK_FREE(0x0006));
    printf("IS_PREV_BLOCK_FREE(0x0007) = %d\n", IS_PREV_BLOCK_FREE(0x0007));
    printf("IS_PREV_BLOCK_FREE(0x0008) = %d\n", IS_PREV_BLOCK_FREE(0x0008));
    printf("IS_PREV_BLOCK_FREE(0x0009) = %d\n", IS_PREV_BLOCK_FREE(0x0009));
}

void test_BLOCK_SIZE(){
    printf("BLOCK_SIZE(0x0003) = %d\n", BLOCK_SIZE(0x0003));
    printf("BLOCK_SIZE(0x0004) = %d\n", BLOCK_SIZE(0x0004));
    printf("BLOCK_SIZE(0x0005) = %d\n", BLOCK_SIZE(0x0005));
    printf("BLOCK_SIZE(0x0006) = %d\n", BLOCK_SIZE(0x0006));
    printf("BLOCK_SIZE(0x0007) = %d\n", BLOCK_SIZE(0x0007));
    printf("BLOCK_SIZE(0x0008) = %d\n", BLOCK_SIZE(0x0008));
    printf("BLOCK_SIZE(0x0009) = %d\n", BLOCK_SIZE(0x0009));
    printf("BLOCK_SIZE(0x00010) = %d\n", BLOCK_SIZE(0x00010));
    printf("BLOCK_SIZE(0x00011) = %d\n", BLOCK_SIZE(0x00011));
    printf("BLOCK_SIZE(0x00012) = %d\n", BLOCK_SIZE(0x00012));
    printf("BLOCK_SIZE(0x00013) = %d\n", BLOCK_SIZE(0x00013));
    printf("BLOCK_SIZE(0x00014) = %d\n", BLOCK_SIZE(0x00014));
    printf("BLOCK_SIZE(0x00015) = %d\n", BLOCK_SIZE(0x00015));
    printf("BLOCK_SIZE(0x00016) = %d\n", BLOCK_SIZE(0x00016));
    printf("BLOCK_SIZE(0x00017) = %d\n", BLOCK_SIZE(0x00017));
}

int main(int argc, char **argv){
    test_IS_USED();
    test_IS_PREV_BLOCK_FREE();
    test_BLOCK_SIZE();
}

