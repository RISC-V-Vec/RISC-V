#include "utils.h"
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
        vint32m1_t zero_vector = __riscv_vmv_v_x_i32m1(0, vl);
        vint32m1_t raw_values = __riscv_vle32_v_i32m1(V, vl);
        vint32m1_t relu_values = __riscv_vmax_vv_i32m1(raw_values, zero_vector, vl);
        __riscv_vse32_v_i32m1(V2, relu_values, vl);
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
