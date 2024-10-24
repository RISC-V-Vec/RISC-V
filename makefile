all:
	riscv64-unknown-linux-gnu-g++ -O3 -march=rv64gcv rvv_add_fast.cpp -lm -static 