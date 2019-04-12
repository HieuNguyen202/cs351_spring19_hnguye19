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
    int i, j, ii, jj;
    int dx = 8;
    int dy = 8;
    for (j = 0; j<M; j+=dy) {
        for (i = 0; i<N; i+=dx) {
            if (i == j) {
                for (jj = 0; jj < dy; ++jj) {
                    for (ii = 0; ii < dx; ++ii) {
                        B[jj+j][i + ((jj + ii + 1)%dx)] = A[i + ((jj + ii + 1)%dx)][jj+j] ;
                    }
                }
            } else {
                for (jj = 0; jj < dy; ++jj) {
                    for (ii = 0; ii < dx; ++ii) {
                        B[jj+j][ii+i] = A[ii+i][jj+j] ;
                    }
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
            A[i][j] = (cidx/block_size)%32;
            cidx++;
        }
    }
    trans1(M, N, A, B);
    printf("Is transposed: %d\n", is_transpose(M, N, A, B));
    print_matrix(M, N, A, B);
}
