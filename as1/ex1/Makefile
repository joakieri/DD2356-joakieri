.PHONY: run test sizes clean

spmv: spmv.out
	./spmv.out 10
	./spmv.out 100
	./spmv.out 1000
	./spmv.out 10000

stream: stream.out
	./stream.out

spmv.out: spmv.c
	cc -O2 -o spmv.out spmv.c

stream.out: stream.c
	cc -O2 -o stream.out stream.c

sizes: sizes.out
	./sizes.out

sizes.out: sizes.c
	cc -o sizes.out sizes.c

clean:
	rm spmv.out stream.out sizes.out
