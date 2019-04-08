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
            printf("%d\t", A[i][j]);
        }
        printf("]\n");
    }

    printf("Matrix B:\n");
    for (i = 0; i < N; i++) {
        printf("[");
        for (j = 0; j < M; ++j) {
            printf("%d\t", B[i][j]);
        }
        printf("]\n");
    }
    return 1;
}

void trans1(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, k, l;
    int block_size_int = 4;
    int block_size_row = 4;
    for (j = M - block_size_int; j > -1; j -= block_size_int) {
        for (i =0; i < N; i += block_size_row) {
//            if (i == j) {
                int lastk = i + block_size_row;
                for (k = i; k < lastk; ++k) {
                    int lastj = j + block_size_int;
                    for (l = j ; l < lastj; ++l) {
                        B[l][k] = A[k][l];
                    }
                }
//            } else {
//                for (k = 0; k < block_size_row; ++k) {
//                    int lastl = j+ block_size_int;
//                    for (l = 0; l < lastl; ++l) {
//                        B[l][k] = A[k][l];
//                    }
//                }
//            }
        }
    }
}

int main(int argc, char** argv) {
    int M = 32;
    int N = 32;
    int A[32][32];
    int B[32][32];
    for (int i = 0; i < 32; ++i) {
        for (int j = 0; j < 32; ++j) {
            A[i][j] = i;
        }
    }
    trans1(M, N, A, B);
    printf("Is transposed: %d\n", is_transpose(M, N, A, B));
    print_matrix(M, N, A, B);
}
