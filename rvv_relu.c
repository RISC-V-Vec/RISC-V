#include "utils.h"
#define USE_32BIT_INT
#include "defines.h"
#include <riscv_vector.h>

const int SIZE = 256;

void relu_golden(int *V, int *V2, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        V2[i] = V[i] > 0 ? V[i] : 0;
    }
}

void relu(int *V, int *V2, int size)
{

    for (size_t vl; size > 0; size -= vl, V += vl, V2 += vl)
    {
        vl = __riscv_vsetvl_e32m1(size);
        vec zero_vector = zero_init(vl);
        vec raw_values = vec_load(V, vl);
        vec relu_values = vec_max(raw_values, zero_vector, vl);
        vec_store(V2, relu_values, vl);
    }
}

int main()
{
    uint32_t seed = 0xdeadbeef;
    srand(seed);

    int V[SIZE];
    int golden[SIZE];
    int actual[SIZE];
    fillArray1d(V, SIZE);
    relu_golden(V, golden, SIZE);
    relu(V, actual, SIZE);
    for (int i = 0; i < SIZE; ++i)
        if (golden[i] != actual[i])
        {
            printf("error");
            exit(-1);
        }
}
