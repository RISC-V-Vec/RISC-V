#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <riscv_vector.h>

#define N 512

float input[N];
float output_golden[N];

float output[N];

void saxpy_golden(size_t n, const float a, const float *x, float *y)
{
  for (size_t i = 0; i < n; ++i)
  {
    y[i] = a * x[i] + y[i];
  }
}

// reference https://github.com/riscv/riscv-v-spec/blob/master/example/saxpy.s
void saxpy_vec(size_t n, const float a, const float *x, float *y)
{

  // prepwork to measure elapsed time
  struct timespec start, end;

  clock_gettime(CLOCK_REALTIME, &start);

  for (size_t vl; n > 0; n -= vl, x += vl, y += vl)
  {
    // vsetvli allows for stripmining, The application specifies the total number of elements to be processed (the application vector length or AVL) as a candidate value for vl,
    // and the hardware responds via a general-purpose register with the (frequently smaller) number of elements that the hardware will handle per iteration
    // e32 -> element width 32 (float)
    // m8 -> LMUL size 8
    // expected VL -> VLEN/32 * 8, 32 (31) locally, still 31 on bananapi because we are capped at n
    vl = __riscv_vsetvl_e32m8(n);
    printf("%ld\n", vl);
    // vfloat32m8_t is the datatype for a vector of float32 with EMUL = 8
    // __riscv_vle32_v_f32m8 -> __riscv is the prefix that is always present, vle -> vector strided load of SEW 32 LMUL 8, base addr + number of elements to read
    vfloat32m8_t vx = __riscv_vle32_v_f32m8(x, vl);
    vfloat32m8_t vy = __riscv_vle32_v_f32m8(y, vl);
    // __riscv_vfmacc_vf_f32m8 -> multiply + accumulate aka fmadd, as always e32 for floats32 and m8 aka lmul 8
    // https://dzaima.github.io/intrinsics-viewer/#0q1YqVbJSKsosTtYtU9JRSoVzFMvSchOTk@PL0uLTjI1yLYCSiUpW0UplSrE6SskgFohRDFQfHw/SURaPqSETKGsIAkq1AA

    // store into y the result of a fmadd with a * vx + vy
    __riscv_vse32_v_f32m8(y, __riscv_vfmacc_vf_f32m8(vy, a, vx, vl), vl);
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  // time_spent = end - start
  double time_spent = (end.tv_sec - start.tv_sec) +
                      (end.tv_nsec - start.tv_nsec);

  printf("Time elpased is %f ns", time_spent);
}

void saxpy_SLA(size_t n, const float a, const float *x, float *y)
{
  const int VLEN = 128;
  const int vl = VLEN / 32 * 8;
  // prepwork to measure elapsed time
  struct timespec start, end;

  clock_gettime(CLOCK_REALTIME, &start);
  for (size_t vl; n > 0; n -= vl, x += vl, y += vl)
  {
    // vfloat32m8_t is the datatype for a vector of float32 with EMUL = 8
    // __riscv_vle32_v_f32m8 -> __riscv is the prefix that is always present, vle -> vector strided load of SEW 32 LMUL 8, base addr + number of elements to read
    vfloat32m8_t vx = __riscv_vle32_v_f32m8(x, vl);
    vfloat32m8_t vy = __riscv_vle32_v_f32m8(y, vl);
    // __riscv_vfmacc_vf_f32m8 -> multiply + accumulate aka fmadd, as always e32 for floats32 and m8 aka lmul 8
    // https://dzaima.github.io/intrinsics-viewer/#0q1YqVbJSKsosTtYtU9JRSoVzFMvSchOTk@PL0uLTjI1yLYCSiUpW0UplSrE6SskgFohRDFQfHw/SURaPqSETKGsIAkq1AA

    // store into y the result of a fmadd with a * vx + vy
    __riscv_vse32_v_f32m8(y, __riscv_vfmacc_vf_f32m8(vy, a, vx, vl), vl);
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  // time_spent = end - start
  double time_spent = (end.tv_sec - start.tv_sec) +
                      (end.tv_nsec - start.tv_nsec);

  printf("Time elpased is %f ns", time_spent);
}

int fp_eq(float reference, float actual, float relErr)
{
  // if near zero, do absolute error instead.
  float absErr = relErr * ((fabsf(reference) > relErr) ? fabsf(reference) : relErr);
  return fabsf(actual - reference) < absErr;
}

int main()
{
  // Initialize arrays
  gen_rand_1d(input, N);
  gen_rand_1d(output, N);
  memcpy(output_golden, output, N);
  saxpy_vec(N, 55.66, input, output_golden);
  saxpy_SLA(N, 55.66, input, output);
  int pass = 1;
  for (int i = 0; i < N; i++)
  {
    if (!fp_eq(output_golden[i], output[i], 1e-6))
    {
      printf("fail, %f=!%f\n", output_golden[i], output[i]);
      pass = 0;
    }
  }
  if (pass)
    printf("pass\n");
  return (pass == 0);
}
