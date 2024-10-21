all:
	riscv64-linux-gnu-gcc -O3 -Ofast -march=rv64gcv rvv_relu.c -lm -static