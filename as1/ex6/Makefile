FLAGS=-O3 -fopt-info-vec-missed=info.txt 

matrix_multiply.out: matrix_multiply.c
	gcc $(FLAGS) -o $@ $<

clean:
	rm matrix_multiply.out info.txt
