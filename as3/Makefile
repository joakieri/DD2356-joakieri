CC=cc

OUT1=ex1/hw.out
OUT2=ex2/domainDecom1D.out
OUT3=$(patsubst %,ex3/%.out,ping_pong_2s ping_pong_1s)
OUT4=$(patsubst %,ex4/%.out,pi_seq pi_seq1 pi_seq2 pi_seq3 pi_seq4)

.PHONY:clean

# Compilation on Bescow
# cray-mpich module loaded by default (check with module list)
beskow: $(OUT1) $(OUT2) $(OUT3) $(OUT4)

# Running on Bescow
# salloc --nodes=1 -t 00:05:00 -A edu21.DD2356 -C Haswell
# srun -n 4 ./a.out

# For ex3
# salloc --nodes=2 -t 00:10:00 -A edu21.DD2356 -C Haswell
# srun -n 2 --ntasks-per-node=1 ./ping_pong

local: CC=mpicc
local: $(OUT1) $(OUT2) $(OUT3) $(OUT4)

run1: local
	mpiexec -n 4 ./$(OUT1)
	
run2: local
	mpiexec -n 4 ./$(OUT2)
	
run3: local
	for p in $(OUT3); do mpiexec -n 2 ./$$p; done

run4: local
	for p in $(OUT4); do mpiexec -n 8 ./$$p; done

clean:
	rm ex1/*.out ex2/*.out ex3/*.out ex4/*.out

ex1/%.out: ex1/%.c
	$(CC) -o $@ $<

ex2/%.out: ex2/%.c
	$(CC) -o $@ $< -lm
	
ex3/%.out: ex3/%.c
	$(CC) -o $@ $<
	
ex4/%.out: ex4/%.c
	$(CC) -o $@ $<	


