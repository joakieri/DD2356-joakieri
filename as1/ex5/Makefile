.PHONY:build test test1 test2 test3 test4 clean

build: transpose_64.out transpose_128.out transpose_2048.out transpose_2049.out 

test: test1 test2 test3 test4

test1: transpose_64.out
	perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads ./$^

test2: transpose_128.out
	perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads ./$^

test3: transpose_2048.out
	perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads ./$^

test4: transpose_2049.out
	perf stat -e instructions,cycles,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads ./$^

clean:
	rm transpose_64.out
	rm transpose_128.out
	rm transpose_2048.out
	rm transpose_2049.out

transpose_64.out: transpose.c
	cc -D N=64 -O2 -o $@ $^ 

transpose_128.out: transpose.c
	cc -D N=128 -O2 -o $@ $^ 

transpose_2048.out: transpose.c
	cc -D N=2048 -O2 -o $@ $^ 

transpose_2049.out: transpose.c
	cc -D N=2049 -O2 -o $@ $^ 


