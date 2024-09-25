#include <riscv_vector.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "common.h"

#define N 1024

void sum_arrays_golden(int *A, int *B, int *golden_sum, int size)
{
  for (int i = 0; i < size; i++)
  {
    golden_sum[i] = A[i] + B[i];
  }
}

void sum_arrays_vec(int *A, int *B, int *vec_sum, int size)
{
  // stripmining
  for (size_t vl; size > 0; size -= vl, A += vl, B += vl, vec_sum += vl)
  {
    vl = __riscv_vsetvl_e32m1(size);
    // Fetch vl count elements
    vint32m1_t vA = __riscv_vle32_v_i32m1(A, vl);
    vint32m1_t vB = __riscv_vle32_v_i32m1(B, vl);
    __riscv_vse32_v_i32m1(vec_sum, __riscv_vadd_vv_i32m1(vA, vB, vl), vl);
  }
}

int main()
{
  bool pass = true;
  int A[N];
  gen_rand_1d_int(A, N);
  int B[N];
  gen_rand_1d_int(B, N);
  int golden_sum[N];
  sum_arrays_golden(A, B, golden_sum, N);
  int vec_sum[N];
  sum_arrays_vec(A, B, vec_sum, N);
  // Check for correctness
  for (int i = 0; i < N; i++)
  {
    if (golden_sum[i] != vec_sum[i])
      pass = false;
  }
  printf("%d\n", pass);
}
