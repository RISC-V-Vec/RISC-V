CC = riscv64-linux-gnu-gcc

scalar :
	$(CC) -O3 -Ofast -march=rv64gcv -static -fno-tree-vectorize -fno-tree-loop-vectorize -fno-tree-slp-vectorize dummy.c tsc.c -lm -o tsvc-sca

vector:
	$(CC) -O3 -Ofast -march=rv64gcv -static  dummy.c tsc.c -lm -o tsvc-vec

VLS:
	clang -mrvv-vector-bits=256 -O3 -Ofast -march=rv64gcv -static  dummy.c tsc.c -lm -o tsvc-vec