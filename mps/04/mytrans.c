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
    fpurge(stdout);
    int i, j;
    printf("Matrix A:\n");
    for (j = 0; j < N; ++j) {
        printf("[");
        for (i = 0; i < M; i++) {
            printf("%2d\t", A[j][i]);
        }
        printf("]\n");
    }

    printf("Matrix B:\n");
    for (j = 0; j < M; j++) {
        printf("[");
        for (i = 0; i < N; ++i) {
            printf("%2d\t", B[j][i]);
        }
        printf("]\n");
    }
    return 1;
}

void trans1(int M, int N, int A[N][M], int B[M][N]) {
    int x, xx, xxx, y, yy, yyy, i, ii, dx, ddx, dy, ddy;
    dx = 8;
    dy = 8;
    ddx = dx / 2;
    ddy = dy / 2;

    for (y = 0; y < N; y += dy) {
        for (x = 0; x < M; x += dx) {
            //Blocking level 1:
            //Let 0, 1, 2, 3 represents elements of the same block.
            //The given matrix can be divided into 8x8 (dy x dx) blocks, one of which is as follow:
            //[0 0 0 0 1 1 1 1]
            //[0 0 0 0 1 1 1 1]
            //[0 0 0 0 1 1 1 1]
            //[0 0 0 0 1 1 1 1]
            //[3 3 3 3 2 2 2 2]
            //[3 3 3 3 2 2 2 2]
            //[3 3 3 3 2 2 2 2]
            //[3 3 3 3 2 2 2 2]
            //In blocking level 1, matrix A, we will load 4x4 (ddy x ddx)sub-blocks in this order: 0, 1, 2, 3
            //Each 4x4 sub-block is processed in the next two inner for loops.
            for (yy = 0; yy < dy; yy += ddy) {
                for (i = 0; i < dx; i += ddx) {
                    xx = (yy / ddy) == 1 ? ddx - i : i;
                    //Blocking level 2
                    //Each 4x4 sub-block of matrix A is loaded in the following order to optimize cache at the diagonal blocks.
                    //[3  0  1  2]
                    //[6  7  4  5]
                    //[9  10 11 8]
                    //[12 12 14 15]
                    for (yyy = 0; yyy < ddy; ++yyy) {
                        for (ii = 0; ii < ddx; ++ii) {
                            xxx = (ii + yyy + 1) % ddx;
                            B[x + xx + xxx][y + yy + yyy] = A[y + yy + yyy][x + xx + xxx];
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    int M = 64;
    int N = 64;
    int A[N][M];
    int B[M][N];
    int cidx = 0;
    int block_size = 8;
    int cache_size = 32;
        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < M; ++i) {
                A[j][i] = j;
                B[i][j] = 0;
//                A[j][i] = (cidx / block_size) % 32;
//                cidx++;
            }
        }
    trans1(M, N, A, B);
    printf("Is transposed: %d\n", is_transpose(M, N, A, B));
    print_matrix(M, N, A, B);
}
