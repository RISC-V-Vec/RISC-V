#include "utils.h"
#include <riscv_vector.h>
#include <chrono>
#include <iostream>

void preprocessVecB(int **B, int n, int m)
{
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
        {
            B[i][j] = log2(B[i][j]);
        }
}

// matrix multiplication
// A[n][o], B[m][o] --> C[n][m];
void matmul_golden(int **a, int **b, int **c, int n, int m, int o)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
        {
            c[i][j] = 0;
            for (int k = 0; k < o; ++k)
                c[i][j] += a[i][k] * b[j][k];
        }
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Scalar code: ";
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << std::endl;
}

void matmul(int **a, int **b, int **c, int n, int m, int o)
{
    auto start = std::chrono::high_resolution_clock::now();
    size_t vlmax = __riscv_vsetvlmax_e32m1();
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            int *ptr_a = &a[i][0];
            int *ptr_b = &b[j][0];
            int k = o;
            vint32m1_t vec_s = __riscv_vmv_v_x_i32m1(0, vlmax);
            vint32m1_t vec_zero = __riscv_vmv_v_x_i32m1(0, vlmax);
            for (size_t vl; k > 0; k -= vl, ptr_a += vl, ptr_b += vl)
            {
                vl = __riscv_vsetvl_e32m1(k);
                vint32m1_t vec_a = __riscv_vle32_v_i32m1(ptr_a, vl);
                vint32m1_t vec_b = __riscv_vle32_v_i32m1(ptr_b, vl);
                vec_s = __riscv_vmacc_vv_i32m1(vec_s, vec_a, vec_b, vl);
            }
            int sum = __riscv_vmv_x_s_i32m1_i32(__riscv_vredsum_vs_i32m1_i32m1(vec_s, vec_zero, vlmax));
            c[i][j] = sum;
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Vector code: ";
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << std::endl;
}

int main()
{
    const int N = 16;
    const int M = 16;
    const int O = 12;
    uint32_t seed = 0xdeadbeef;
    srand(seed);

    // data gen
    int **A = alloc_array_2d(N, O);
    int **B = alloc_array_2d(M, O);
    pow2fill(A, N, O);
    pow2fill(B, M, O);

    // compute
    int **golden = alloc_array_2d(N, M);
    int **actual = alloc_array_2d(N, M);
    matmul_golden(A, B, golden, N, M, O);
    matmul(A, B, actual, N, M, O);

    // compare
    puts(compare_2d(golden, actual, N, M) ? "pass" : "fail");
}
