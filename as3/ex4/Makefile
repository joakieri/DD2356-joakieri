N=8

default:
	cc -o pi_seq.out pi_seq.c -lm
	./pi_seq.out

test1:
	mpicc -o pi_seq1.out pi_seq1.c
	mpiexec -n $(N) ./pi_seq1.out

test2:
	mpicc -o pi_seq2.out pi_seq2.c
	mpiexec -n $(N) ./pi_seq2.out

test3:
	mpicc -o pi_seq3.out pi_seq3.c
	mpiexec -n $(N) ./pi_seq3.out

test4:
	mpicc -o pi_seq4.out pi_seq4.c
	mpiexec -n $(N) ./pi_seq4.out

clean:
	rm *.out
