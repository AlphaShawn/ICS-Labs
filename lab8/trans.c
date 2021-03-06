/**
 * Lab8: Part B
 * Student ID: 5140379066
 * Name      : Xiao YuWei
 *
 * Strategy:
 *     64X64:
 *         1.对角线上8X8大小的单独处理，分左上，左下，右上，右下四个4X4的小方块处理。
 *         2.其余部分8X8大小方块按如下方式处理：
 *             (1)处理左上左下两个4X4方块，同时记录右上方块的第一第二行的数据（在读左上方块时候会被缓存）
 *             (2)把缓存的2X4数据放到指定位置，处理右下4X4方块，处理右上剩余2X4方块
 *     32X32, 61X67:
 *         1.把矩阵分割为若干个8X8的小方块，分别对小方块处理
 *         2.对每个小方块，处理每一行时，直接缓存一整行的数据（至多MISS一次），然后一起往指定位置写入（出了第一次MISS8次，之后每次MISS至多1次）。
 */


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
    /*local variable, total:12 */
    int ii, jj, i, j, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    /*单独处理64X64*/
    if(M == 64 && N == 64)
    {
        /*diagnose*/
        for(jj = 0; jj < (N+3)/4; jj++)
        {
            for(ii = (jj>>1)<<1; ii<=(jj|1); ii++)
            {
                for(i = ii*4; i < (ii+1)*4 && i < N; i=i+2)
                {
		            /*一次处理2行*/
                    if( jj*4 + 1 < M)
                        tmp1 = A[i][jj*4+1];
                    if( jj*4 + 2 < M)
                        tmp2 = A[i][jj*4+2];
                    if( jj*4 + 3 < M)
                        tmp3 = A[i][jj*4+3];
                    if( i+1 < N)
                    {    
                        if( jj*4 < M)
                            tmp4 = A[i+1][jj*4];
                        if( jj*4 + 1 < M)
                            tmp5 = A[i+1][jj*4+1];
                        if( jj*4 + 2 < M)
                            tmp6 = A[i+1][jj*4+2];
                        if( jj*4 + 3 < M)
                            tmp7 = A[i+1][jj*4+3];
                    }
                    B[jj*4][i] = A[i][jj*4];
                    if( jj*4 + 1 < M)
                        B[jj*4+1][i] = tmp1;
                    if( jj*4 + 2 < M)
                        B[jj*4+2][i] = tmp2;
                    if( jj*4 + 3 < M)
                        B[jj*4+3][i] = tmp3;
                    if( i+1 < N)
                    {
                        if( jj*4 < M)
                            B[jj*4][i+1] = tmp4;
                        if( jj*4 + 1 < M)
                            B[jj*4+1][i+1] = tmp5;
                        if( jj*4 + 2 < M)
                            B[jj*4+2][i+1] = tmp6;
                        if( jj*4 + 3 < M)
                            B[jj*4+3][i+1] = tmp7;
                    }
                }
            }
        }

        /*rest element*/
        for(ii = 0; ii < (N+3)/4; ii+=2)
        {
            for(jj = 0; jj < (M+3)/4; jj+=2)
            {
		        /*对角线方块跳过*/
                if( (ii>>1) == (jj >> 1))
                    continue;
                for(i = ii*4; i<(ii+2)*4 && i<N; i++)
                {
		            /*直接复制，用local variable记录右上方块的第一第二行缓存*/
                    for(j = jj*4; j<(jj+1)*4 && j<M; j++)
                    {
                        B[j][i]=A[i][j];
                    }
                    if(i == ii*4)
                    {
                        tmp0 = A[i][jj*4+4];
                        tmp1 = A[i][jj*4+5];
                        tmp2 = A[i][jj*4+6];
                        tmp3 = A[i][jj*4+7];
                    }
                    if(i == ii*4+1)
                    {
                        tmp4 = A[i][jj*4+4];
                        tmp5 = A[i][jj*4+5];
                        tmp6 = A[i][jj*4+6];
                        tmp7 = A[i][jj*4+7];
                    }
                }
		        /*缓存下来的数据直接赋值*/
                i = ii*4;
                B[jj*4+4][i] = tmp0;
                B[jj*4+5][i] = tmp1;
                B[jj*4+6][i] = tmp2;
                B[jj*4+7][i] = tmp3;
                B[jj*4+4][i+1] = tmp4;
                B[jj*4+5][i+1] = tmp5;
                B[jj*4+6][i+1] = tmp6;
                B[jj*4+7][i+1] = tmp7;
		        /*处理右下*/
                for(i = (ii+1)*4; i<(ii+2)*4 && i<N; i++)
                {
                    for(j=(jj+1)*4; j<(jj+2)*4 && j<M; j++)
                    {
                        B[j][i]=A[i][j];
                    }
                }
		        /*处理右上剩余2行*/
                for(i = ii*4+2; i<(ii+1)*4 && i<N; i++)
                {
                    for(j=(jj+1)*4; j<(jj+2)*4 && j<M; j++)
                    {
                        B[j][i]=A[i][j];
                    }
                }
            }
        }
    }
    else 					/*处理32X32和61X67*/
    {
        for(ii = 0; ii < (N+7)/8; ii++)
        {
            for(jj = 0; jj < (M+7)/8; jj++)
            {
                for(i = ii*8; i < (ii+1)*8 && i < N; i++)
                {
		            /*把一行8个元素一次性缓存到local variable*/
                    if( jj*8 + 1 < M)
                        tmp1 = A[i][jj*8+1];
                    if( jj*8 + 2 < M)
                        tmp2 = A[i][jj*8+2];
                    if( jj*8 + 3 < M)
                        tmp3 = A[i][jj*8+3];
                    if( jj*8 + 4 < M)
                        tmp4 = A[i][jj*8+4];
                    if( jj*8 + 5 < M)
                        tmp5 = A[i][jj*8+5];
                    if( jj*8 + 6 < M)
                        tmp6 = A[i][jj*8+6];
                    if( jj*8 + 7 < M)
                        tmp7 = A[i][jj*8+7];
		            /*一次性赋值给B*/
                    B[jj*8][i] = A[i][jj*8];
                    if( jj*8 + 1 < M)
                        B[jj*8+1][i] = tmp1;
                    if( jj*8 + 2 < M)
                        B[jj*8+2][i] = tmp2;
                    if( jj*8 + 3 < M)
                        B[jj*8+3][i] = tmp3;
                    if( jj*8 + 4 < M)
                        B[jj*8+4][i] = tmp4;
                    if( jj*8 + 5 < M)
                        B[jj*8+5][i] = tmp5;
                    if( jj*8 + 6 < M)
                        B[jj*8+6][i] = tmp6;
                    if( jj*8 + 7 < M)
                        B[jj*8+7][i] = tmp7;
                }
            }
        }
    }
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
    //registerTransFunction(transpose_submit, transpose_submit_desc); 

    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

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

