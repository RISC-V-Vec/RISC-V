all:
	riscv64-linux-gnu-gcc -march=rv64gcv -static rvv_saxpy.c -o saxpy