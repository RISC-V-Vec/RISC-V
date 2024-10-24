#include <iostream>
#include <vector>
#include <chrono>
#include <riscv_vector.h>

// Function to add vectors using RVV intrinsics
void vector_add_rvv(const std::vector<int32_t> &a, const std::vector<int32_t> &b, std::vector<int32_t> &c)
{
    size_t n = a.size();
    size_t vl;
    for (size_t i = 0; i < n; i += vl)
    {
        vl = __riscv_vsetvl_e32m1(n - i);
        vint32m1_t va = __riscv_vle32_v_i32m1(&a[i], vl);
        vint32m1_t vb = __riscv_vle32_v_i32m1(&b[i], vl);
        vint32m1_t vc = __riscv_vadd_vv_i32m1(va, vb, vl);
        __riscv_vse32_v_i32m1(&c[i], vc, vl);
    }
}

// Function to add vectors using scalar operations
void vector_add_scalar(const std::vector<int32_t> &a, const std::vector<int32_t> &b, std::vector<int32_t> &c)
{
    size_t n = a.size();
    for (size_t i = 0; i < n; ++i)
    {
        c[i] = a[i] + b[i];
    }
}

// Function to add vectors using RVV intrinsics
void vector_mul_rvv(const std::vector<int32_t> &a, const std::vector<int32_t> &b, std::vector<int32_t> &c)
{
    size_t n = a.size();
    size_t vl;
    for (size_t i = 0; i < n; i += vl)
    {
        vl = __riscv_vsetvl_e32m1(n - i);
        vint32m1_t va = __riscv_vle32_v_i32m1(&a[i], vl);
        vint32m1_t vb = __riscv_vle32_v_i32m1(&b[i], vl);
        vint32m1_t vc = __riscv_vmul_vv_i32m1(va, vb, vl);
        __riscv_vse32_v_i32m1(&c[i], vc, vl);
    }
}

// Function to add vectors using scalar operations
void vector_mul_scalar(const std::vector<int32_t> &a, const std::vector<int32_t> &b, std::vector<int32_t> &c)
{
    size_t n = a.size();
    for (size_t i = 0; i < n; ++i)
    {
        c[i] = a[i] * b[i];
    }
}

// Function to add vectors using RVV intrinsics
void vector_shift_rvv(const std::vector<int32_t> &a, const std::vector<uint32_t> &b, std::vector<int32_t> &c)
{
    size_t n = a.size();
    size_t vl;
    for (size_t i = 0; i < n; i += vl)
    {
        vl = __riscv_vsetvl_e32m1(n - i);
        vint32m1_t va = __riscv_vle32_v_i32m1(&a[i], vl);
        vuint32m1_t vb = __riscv_vle32_v_u32m1(&b[i], vl);
        vint32m1_t vc = __riscv_vsll_vv_i32m1(va, vb, vl);
        __riscv_vse32_v_i32m1(&c[i], vc, vl);
    }
}

// Function to add vectors using scalar operations
void vector_shift_scalar(const std::vector<int32_t> &a, const std::vector<uint32_t> &b, std::vector<int32_t> &c)
{
    size_t n = a.size();
    for (size_t i = 0; i < n; ++i)
    {
        c[i] = a[i] << b[i];
    }
}

// Function to verify the results of scalar and RVV methods
void verify_results(const std::vector<int32_t> &scalar_result, const std::vector<int32_t> &rvv_result)
{
    if (scalar_result == rvv_result)
    {
        std::cout << "Results match" << std::endl;
    }
    else
    {
        std::cout << "Results mismatch" << std::endl;
    }
}

int main()
{
    const size_t N = 1000000; // Size of the vectors
    std::vector<int32_t> a(N, 1), b(N, 2), c_rvv(N, 0), c_scalar(N, 0);

    // Measure time for RVV vector addition
    auto start_rvv = std::chrono::high_resolution_clock::now();
    vector_add_rvv(a, b, c_rvv);
    auto end_rvv = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_rvv = end_rvv - start_rvv;
    std::cout << "RVV Vector Addition Time: " << duration_rvv.count() << " seconds" << std::endl;

    // Measure time for scalar vector addition
    auto start_scalar = std::chrono::high_resolution_clock::now();
    vector_add_scalar(a, b, c_scalar);
    auto end_scalar = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_scalar = end_scalar - start_scalar;
    std::cout << "Scalar Vector Addition Time: " << duration_scalar.count() << " seconds" << std::endl;

    // Verify results
    verify_results(c_scalar, c_rvv);

    // Measure time for RVV vector multiplication
    start_rvv = std::chrono::high_resolution_clock::now();
    vector_mul_rvv(a, b, c_rvv);
    end_rvv = std::chrono::high_resolution_clock::now();
    duration_rvv = end_rvv - start_rvv;
    std::cout << "RVV Vector Multiplication Time: " << duration_rvv.count() << " seconds" << std::endl;

    // Measure time for scalar vector addition
    start_scalar = std::chrono::high_resolution_clock::now();
    vector_mul_scalar(a, b, c_scalar);
    end_scalar = std::chrono::high_resolution_clock::now();
    duration_scalar = end_scalar - start_scalar;
    std::cout << "Scalar Vector Multiplication Time: " << duration_scalar.count() << " seconds" << std::endl;

    // Verify results
    verify_results(c_scalar, c_rvv);

    // For shifts get an unsigned vector
    std::vector<uint32_t> shifts(N, 2);

    // Measure time for RVV vector shift
    start_rvv = std::chrono::high_resolution_clock::now();
    vector_shift_rvv(a, shifts, c_rvv);
    end_rvv = std::chrono::high_resolution_clock::now();
    duration_rvv = end_rvv - start_rvv;
    std::cout << "RVV Vector Shift Time: " << duration_rvv.count() << " seconds" << std::endl;

    // Measure time for scalar vector shift
    start_scalar = std::chrono::high_resolution_clock::now();
    vector_shift_scalar(a, shifts, c_scalar);
    end_scalar = std::chrono::high_resolution_clock::now();
    duration_scalar = end_scalar - start_scalar;
    std::cout << "Scalar Vector Shift Time: " << duration_scalar.count() << " seconds" << std::endl;

    return 0;
}
