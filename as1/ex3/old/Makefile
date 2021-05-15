#NUM=2147483647 
#NUM=2147483648 
NUM=5000
#FLAGS=
FLAGS=-O2

.PHONY: build asm cg clean

build: benchmark.out clockgranularity.out

bm: benchmark.out
	./benchmark.out
	./benchmark.out
	./benchmark.out
	./benchmark.out
	./benchmark.out

asm: benchmark.s benchmarkO2.s

cg: clockgranularity.out
	./clockgranularity.out 100 

clean:
	rm benchmark.out benchmarkO2.s benchmark.s clockgranularity.out

benchmark.out: benchmark.c
	cc $(FLAGS) -D N=$(NUM) -o $@ $<

clockgranularity.out: clockgranularity.c
	cc -o $@ $<

benchmark.s: benchmark.c
	cc -S -fverbose-asm -D N=$(NUM) -o $@ $<

benchmarkO2.s: benchmark.c
	cc -S -fverbose-asm -O2 -D N=$(NUM) -o $@ $<
