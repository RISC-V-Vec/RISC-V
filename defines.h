#if defined(USE_32BIT_INT)
#define vec vint32m1_t
#define zero_init(vl) __riscv_vmv_v_x_i32m1(0, vl)
#define vec_load(src, size) __riscv_vle32_v_i32m1(src, size)
#define vec_store(dst, values, size) __riscv_vse32_v_i32m1(dst, values, size)
#define vec_max(op1, op2, vl) __riscv_vmax_vv_i32m1(op1, op2, vl);
#endif