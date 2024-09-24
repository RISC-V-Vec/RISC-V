all:
	riscv64-linux-gnu-gcc -march=rv64gcv -static rvv_matmul.c -o matmul