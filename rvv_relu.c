#include "utils.h"
#define USE_16BIT_FP
#include "defines.h"
#include <riscv_vector.h>

const int SIZE = 256;

void relu_golden(data_type *V, data_type *V2, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        V2[i] = V[i] > 0 ? V[i] : 0;
    }
}

void relu(data_type *V, data_type *V2, int size)
{
    size_t vlmax = __riscv_vsetvlmax_e32m1();
    vec zero_vector = zero_init(vlmax);
    for (size_t vl; size > 0; size -= vl, V += vl, V2 += vl)
    {
        vl = get_vl(size);
        vec raw_values = vec_load(V, vl);
        vec relu_values = vec_max(raw_values, zero_vector, vl);
        vec_store(V2, relu_values, vl);
    }
}

void fillArray1d(data_type *V, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        V[i] = (data_type)rand();
    }
}

int main()
{
    uint32_t seed = 0xdeadbeef;
    srand(seed);
    data_type V[SIZE];
    data_type golden[SIZE];
    data_type actual[SIZE];
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
