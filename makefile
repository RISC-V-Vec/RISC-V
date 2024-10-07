all:
	riscv64-linux-gnu-gcc -O3 -Ofast -march=rv64gcv -static rvv_add.c -o add