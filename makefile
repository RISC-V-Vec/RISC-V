all:
	riscv64-unknown-linux-gnu-g++ -O3 -march=rv64gcv rvv_bench.cpp -lm -static 