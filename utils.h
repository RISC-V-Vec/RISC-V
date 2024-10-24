#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <riscv_vector.h>

void pow2fill(int **ar, int n, int m)
{
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            ar[i][j] = pow(2, (rand() % 16));
}

bool compare_2d(int **golden, int **actual, int n, int m)
{
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (golden[i][j] != actual[i][j])
                return false;
    return true;
}

int **alloc_array_2d(int n, int m)
{
    int **ret;
    ret = (int **)malloc(sizeof(double *) * n);
    for (int i = 0; i < n; ++i)
        ret[i] = (int *)malloc(sizeof(int) * m);
    return ret;
}

void print2darray(int **ar, int n, int m)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
            printf("%d ", ar[i][j]);
        printf("\n");
    }
}

void printArray1d(int *V, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%d ", V[i]);
    }
    printf("\n");
}

void printvectorcontent(int vl, vint32m1_t v)
{
    for (size_t i = 0; i < vl; ++i)
        printf("%d ", __riscv_vmv_x_s_i32m1_i32(__riscv_vslidedown_vx_i32m1(v, i, vl)));
}
