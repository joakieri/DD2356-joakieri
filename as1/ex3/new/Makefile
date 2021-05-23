#NUM=2147483647 
#NUM=2147483648 
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
	rm *.out *.s

benchmark.out: benchmark.c
	cc $(FLAGS) -o $@ $<

clockgranularity.out: clockgranularity.c
	cc -o $@ $<

benchmark.s: benchmark.c
	cc -S -fverbose-asm -o $@ $<

benchmarkO2.s: benchmark.c
	cc -S -fverbose-asm -O2 -o $@ $<
