#include <stddef.h>
#include <stdint.h>

#if defined(USE_8BIT_INT)
typedef vint8m1_t vec;
typedef vuint8m1_t shift_vec;
typedef int8_t data_type;
typedef uint8_t shift_type;
#define zero_init(vl) __riscv_vmv_v_x_i8m1(0, vl)
#define vec_load(src, size) __riscv_vle8_v_i8m1(src, size)
#define shift_vec_load(src, size) __riscv_vle8_v_u8m1(src, size)
#define vec_store(dst, values, size) __riscv_vse8_v_i8m1(dst, values, size)
#define vec_max(op1, op2, vl) __riscv_vmax_vv_i8m1(op1, op2, vl)
#define vec_add(op1, op2, vl) __riscv_vadd_vv_i8m1(op1, op2, vl);
#define vec_mul(op1, op2, vl) __riscv_vmul_vv_i8m1(op1, op2, vl);
#define vec_shift(op1, shift, vl) __riscv_vsll_vv_i8m1(op1, shift, vl);
#define get_vl(size) __riscv_vsetvl_e8m1(size)

#elif defined(USE_16BIT_INT)
typedef vint16m1_t vec;
typedef vuint16m1_t shift_vec;
typedef int16_t data_type;
typedef uint16_t shift_type;
#define zero_init(vl) __riscv_vmv_v_x_i16m1(0, vl)
#define vec_load(src, size) __riscv_vle16_v_i16m1(src, size)
#define shift_vec_load(src, size) __riscv_vle16_v_u16m1(src, size)
#define vec_store(dst, values, size) __riscv_vse16_v_i16m1(dst, values, size)
#define vec_max(op1, op2, vl) __riscv_vmax_vv_i16m1(op1, op2, vl)
#define vec_add(op1, op2, vl) __riscv_vadd_vv_i16m1(op1, op2, vl);
#define vec_mul(op1, op2, vl) __riscv_vmul_vv_i16m1(op1, op2, vl);
#define vec_shift(op1, shift, vl) __riscv_vsll_vv_i16m1(op1, shift, vl);
#define get_vl(size) __riscv_vsetvl_e16m1(size)

#elif defined(USE_32BIT_INT)
typedef vint32m1_t vec;
typedef vuint32m1_t shift_vec;
typedef int32_t data_type;
typedef uint32_t shift_type;
#define zero_init(vl) __riscv_vmv_v_x_i32m1(0, vl)
#define vec_load(src, size) __riscv_vle32_v_i32m1(src, size)
#define shift_vec_load(src, size) __riscv_vle32_v_u32m1(src, size)
#define vec_store(dst, values, size) __riscv_vse32_v_i32m1(dst, values, size)
#define vec_max(op1, op2, vl) __riscv_vmax_vv_i32m1(op1, op2, vl)
#define vec_add(op1, op2, vl) __riscv_vadd_vv_i32m1(op1, op2, vl);
#define vec_mul(op1, op2, vl) __riscv_vmul_vv_i32m1(op1, op2, vl);
#define vec_shift(op1, shift, vl) __riscv_vsll_vv_i32m1(op1, shift, vl);
#define get_vl(size) __riscv_vsetvl_e32m1(size)

#elif defined(USE_32BIT_FP)
typedef vfloat32m1_t vec;
typedef float data_type;
#define zero_init(vl) __riscv_vfmv_v_f_f32m1(0, vl)
#define vec_load(src, size) __riscv_vle32_v_f32m1(src, size)
#define vec_store(dst, values, size) __riscv_vse32_v_f32m1(dst, values, size)
#define vec_max(op1, op2, vl) __riscv_vfmax_vv_f32m1(op1, op2, vl)
#define vec_add(op1, op2, vl) __riscv_vfadd_vv_f32m1(va, vb, vl);
#define vec_mul(op1, op2, vl) __riscv_vfmul_vv_f32m1(va, vb, vl);
#define get_vl(size) __riscv_vsetvl_e32m1(size)
#endif