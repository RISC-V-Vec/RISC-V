#include "utils.h"
#include <riscv_vector.h>

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
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
    {
      c[i][j] = 0;
      for (int k = 0; k < o; ++k)
        c[i][j] += a[i][k] * b[j][k];
    }
}

void matmul(int **a, int **b, int **c, int n, int m, int o)
{
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

      vint32m1_t vec_sum;
      vec_sum = __riscv_vredsum_vs_i32m1_i32m1(vec_s, vec_zero, vlmax);
      int sum = __riscv_vmv_x_s_i32m1_i32(vec_sum);
      c[i][j] = sum;
    }
  }
}

void matmul_shift(int **a, int **b, int **c, int n, int m, int o)
{
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
        vuint32m1_t vec_b = __riscv_vle32_v_i32m1(ptr_b, vl);
        // Problem 1: convert the values of vec_b into indexes for a shif and into an unsigned vec
        // Problem 2: even assuming we get the shift indexes, we don't have a macc equivalent, so we'll need 2 operations

        vint32m1_t vec_mul = __riscv_vsll_vv_i32m1(vec_a, vec_b, vl);
        vec_s = __riscv_vadd_vv_i32m1(vec_s, vec_mul, vl);
      }

      vint32m1_t vec_sum;
      int sum = __riscv_vmv_x_s_i32m1_i32(__riscv_vredsum_vs_i32m1_i32m1(vec_s, vec_zero, vlmax));
      c[i][j] = sum;
    }
  }
}

int main()
{
  const int N = 8;
  const int M = 8;
  const int O = 7;
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

  // Workaround for the log problem
  preprocessVecB(B, M, O);

  matmul_shift(A, B, actual, N, M, O);

  // compare
  puts(compare_2d(golden, actual, N, M) ? "pass" : "fail");
}
