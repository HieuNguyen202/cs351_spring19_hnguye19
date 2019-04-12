#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

int print_matrix(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    printf("Matrix A:\n");
    for (i = 0; i < N; i++) {
        printf("[");
        for (j = 0; j < M; ++j) {
            printf("%2d\t", A[i][j]);
        }
        printf("]\n");
    }

    printf("Matrix B:\n");
    for (i = 0; i < N; i++) {
        printf("[");
        for (j = 0; j < M; ++j) {
            printf("%2d\t", B[i][j]);
        }
        printf("]\n");
    }
    return 1;
}

void trans1(int M, int N, int A[N][M], int B[M][N]) {
    int dx = 8;
    int dy = 8;
    int x, y, xx, yy, i;
    for (y = 0; y < M; y += dy) {
        for (x = 0; x < N; x += dx) {
            //Blocking of size dyXdx
            //Assuming the block is size 4x4, the order that elements from matrix A should be loaded is as follow:
            //[3  0  1  2]
            //[6  7  4  5]
            //[9  10 11 8]
            //[12 12 14 15]
            for (yy = 0; yy < dy; ++yy) {
                for (i = 0; i < dx; ++i) {
                    xx = (i + yy + 1) % dx;
                    B[x+ xx][yy + y] = A[y + yy][x + xx];
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    int M = 32;
    int N = 32;
    int A[N][M];
    int B[N][M];
    int cidx = 0;
    int block_size = 8;
    int cache_size = 32;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            A[i][j] = i*2+j;
//            A[i][j] = (cidx/block_size)%32;
//            cidx++;
        }
    }
    trans1(M, N, A, B);
    printf("Is transposed: %d\n", is_transpose(M, N, A, B));
    print_matrix(M, N, A, B);
}
