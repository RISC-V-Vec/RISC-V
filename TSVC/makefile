CC = gcc

scalar :
	$(CC) -O3 -Ofast -fno-tree-vectorize -fno-tree-loop-vectorize -fno-tree-slp-vectorize dummy.c tsc.c -lm -o tsvc-sca

vector-avx2 :
	$(CC) -O3 -Ofast -march=bdver4 -mno-tbm -mno-sse4a -mno-bmi2 dummy.c tsc.c -lm -o tsvc-vec-avx2

vector-avx512:
	$(CC) -O3 -Ofast -march=x86-64-v4 -mtune=znver4  dummy.c tsc.c -lm -o tsvc-vec-avx512