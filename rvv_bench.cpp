#include <iostream>
#include <vector>
#include <chrono>
#include <riscv_vector.h>
#define USE_32BIT_INT
#include "defines.h"

// Function to add vectors using RVV intrinsics
void vector_add_rvv(const std::vector<data_type> &a, const std::vector<data_type> &b, std::vector<data_type> &c)
{
    size_t n = a.size();
    size_t vl;
    for (size_t i = 0; i < n; i += vl)
    {
        vl = get_vl(n - i);
        vec va = vec_load(&a[i], vl);
        vec vb = vec_load(&b[i], vl);
        vec vc = vec_add(va, vb, vl);
        vec_store(&c[i], vc, vl);
    }
}

// Function to add vectors using scalar operations
void vector_add_scalar(const std::vector<data_type> &a, const std::vector<data_type> &b, std::vector<data_type> &c)
{
    size_t n = a.size();
    for (size_t i = 0; i < n; ++i)
    {
        c[i] = a[i] + b[i];
    }
}

// Function to mul vectors using RVV intrinsics
void vector_mul_rvv(const std::vector<data_type> &a, const std::vector<data_type> &b, std::vector<data_type> &c)
{
    size_t n = a.size();
    size_t vl;
    for (size_t i = 0; i < n; i += vl)
    {
        vl = get_vl(n - i);
        vec va = vec_load(&a[i], vl);
        vec vb = vec_load(&b[i], vl);
        vec vc = vec_mul(va, vb, vl);
        vec_store(&c[i], vc, vl);
    }
}

// Function to add vectors using scalar operations
void vector_mul_scalar(const std::vector<data_type> &a, const std::vector<data_type> &b, std::vector<data_type> &c)
{
    size_t n = a.size();
    for (size_t i = 0; i < n; ++i)
    {
        c[i] = a[i] * b[i];
    }
}

// Function to add vectors using RVV intrinsics
void vector_shift_rvv(const std::vector<data_type> &a, const std::vector<shift_type> &b, std::vector<data_type> &c)
{
    size_t n = a.size();
    size_t vl;
    for (size_t i = 0; i < n; i += vl)
    {
        vl = get_vl(n - i);
        vec va = vec_load(&a[i], vl);
        shift_vec vb = shift_vec_load(&b[i], vl);
        vec vc = vec_shift(va, vb, vl);
        vec_store(&c[i], vc, vl);
    }
}

// Function to add vectors using scalar operations
void vector_shift_scalar(const std::vector<data_type> &a, const std::vector<shift_type> &b, std::vector<data_type> &c)
{
    size_t n = a.size();
    for (size_t i = 0; i < n; ++i)
    {
        c[i] = a[i] << b[i];
    }
}

// Function to verify the results of scalar and RVV methods
void verify_results(const std::vector<data_type> &scalar_result, const std::vector<data_type> &rvv_result)
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
    std::vector<data_type> a(N, 1), b(N, 2), c_rvv(N, 0), c_scalar(N, 0);

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
    std::vector<shift_type> shifts(N, 2);

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

    // Verify results
    verify_results(c_scalar, c_rvv);

    return 0;
}
