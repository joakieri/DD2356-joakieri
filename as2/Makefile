build: hw stream_bw stream_static stream_dynamic stream_guided sum DFTW1 DFTW2

hw: hw.c
	cc -fopenmp -o $@ $^

stream_bw: stream_bw.c
	cc -fopenmp -o $@ $^ -lm

stream_static: stream_static.c
	cc -fopenmp -o $@ $^ -lm

stream_dynamic: stream_dynamic.c
	cc -fopenmp -o $@ $^ -lm

stream_guided: stream_guided.c
	cc -fopenmp -o $@ $^ -lm
	
sum: sum.c
	cc -fopenmp -o $@ $^ -lm
	
DFTW1: DFTW1.c
	cc -O3 -fopenmp -o $@ $^ -lm
	
DFTW2: DFTW2.c
	cc -O3 -fopenmp -o $@ $^ -lm
