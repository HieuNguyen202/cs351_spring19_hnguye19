/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
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

void mytrans(int M, int N, int A[N][M], int B[M][N], int dy, int dx) {
    int i, j, ii, jj, h;
    for (j = 0; j < M; j += dy) {
        for (i = 0; i < N; i += dx) {
            if (i == j) {
                for (jj = 0; jj < dy; ++jj) {
                    for (ii = 0; ii < dx; ++ii) {
                        h = i + ((jj + ii + 1)%dx);
                        B[jj+j][h] = A[h][jj+j] ;
                    }
                }
            } else {
            for (jj = 0; jj < dy; ++jj) {
                for (ii = 0; ii < dx; ++ii) {
                    B[jj + j][ii + i] = A[ii + i][jj + j];
                }
            }
        }
    }
}


char trans1_desc[] = "Blocking transpose 1";
void trans1(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 1;
    int block_size_row = 1;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans2_desc[] = "Blocking transpose 2";
void trans2(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 2;
    int block_size_row = 2;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans3_desc[] = "Blocking transpose 3";
void trans3(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 3;
    int block_size_row = 3;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans4_desc[] = "Blocking transpose 4";
void trans4(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 4;
    int block_size_row = 4;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans5_desc[] = "Blocking transpose 5";
void trans5(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 5;
    int block_size_row = 5;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans6_desc[] = "Blocking transpose 6";
void trans6(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 6;
    int block_size_row = 6;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans7_desc[] = "Blocking transpose 7";
void trans7(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 7;
    int block_size_row = 7;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans8_desc[] = "Blocking transpose 8";
void trans8(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 8;
    int block_size_row = 8;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans9_desc[] = "Blocking transpose 9";
void trans9(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 9;
    int block_size_row = 9;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans10_desc[] = "Blocking transpose 10";
void trans10(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 10;
    int block_size_row = 10;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans11_desc[] = "Blocking transpose 11";
void trans11(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 11;
    int block_size_row = 11;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans12_desc[] = "Blocking transpose 12";
void trans12(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 12;
    int block_size_row = 12;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans13_desc[] = "Blocking transpose 13";
void trans13(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 13;
    int block_size_row = 13;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans14_desc[] = "Blocking transpose 14";
void trans14(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 14;
    int block_size_row = 14;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans15_desc[] = "Blocking transpose 15";
void trans15(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 15;
    int block_size_row = 15;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}
char trans16_desc[] = "Blocking transpose 16";
void trans16(int M, int N, int A[N][M], int B[M][N])
{
    int block_size_int = 16;
    int block_size_row = 16;
    mytrans(M, N, A, B, block_size_row, block_size_int);
}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 
    registerTransFunction(trans1, trans1_desc);
    registerTransFunction(trans2, trans2_desc);
    registerTransFunction(trans3, trans3_desc);
    registerTransFunction(trans4, trans4_desc);
    registerTransFunction(trans5, trans5_desc);
    registerTransFunction(trans6, trans6_desc);
    registerTransFunction(trans7, trans7_desc);
    registerTransFunction(trans8, trans8_desc);
    registerTransFunction(trans9, trans9_desc);
    registerTransFunction(trans10, trans10_desc);
    registerTransFunction(trans11, trans11_desc);
    registerTransFunction(trans12, trans12_desc);
    registerTransFunction(trans13, trans13_desc);
    registerTransFunction(trans14, trans14_desc);
    registerTransFunction(trans15, trans15_desc);
    registerTransFunction(trans16, trans16_desc);
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

