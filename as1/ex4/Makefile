.PHONY:test test1 test2 test3 test4 clean

build: matrix_multiply_64.out matrix_multiply_opt_64.out matrix_multiply.out matrix_multiply_opt.out

# On Beskow
# srun -n 1 perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads ./matrix_multiply_64.out

test: test1 test2 test3 test4

test1: matrix_multiply_64.out
	perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads ./matrix_multiply_64.out

test2: matrix_multiply_opt_64.out
	perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads ./matrix_multiply_opt_64.out

test3: matrix_multiply.out
	perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads ./matrix_multiply.out

test4: matrix_multiply_opt.out
	perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads ./matrix_multiply_opt.out

clean:
	rm *.out 

matrix_multiply_64.out: matrix_multiply.c
	cc -D MSIZE=64 -g -O2 matrix_multiply.c -o matrix_multiply_64.out

matrix_multiply_opt_64.out: matrix_multiply_opt.c
	cc -D MSIZE=64 -g -O2 matrix_multiply_opt.c -o matrix_multiply_opt_64.out

matrix_multiply.out: matrix_multiply.c
	cc -g -O2 matrix_multiply.c -o matrix_multiply.out

matrix_multiply_opt.out: matrix_multiply_opt.c
	cc -g -O2 matrix_multiply_opt.c -o matrix_multiply_opt.out

