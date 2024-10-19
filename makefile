all:
	riscv64-linux-gnu-gcc -O3 -Ofast -march=rv64gcv -static rvv_matmul.c -o matmul -lm