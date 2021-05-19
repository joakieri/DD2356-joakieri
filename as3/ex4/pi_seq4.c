#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[]){

	MPI_Init(&argc, &argv);	

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Win win;

	int count = 0;
	double x, y, z;

	srand(SEED * rank);

	// Calculate PI following a Monte Carlo method
	for (int iter = 0; iter < (NUM_ITER / size); iter++) {
		// Generate random (X,Y) points
		x = (double)random() / (double)RAND_MAX;
		y = (double)random() / (double)RAND_MAX;
		//z = sqrt((x*x) + (y*y));
		z = x*x + y*y;

		// Check if point is in unit circle
		if (z <= 1.0)
			count++;
	}

	if (rank == 0) {
		double start, stop, diff;
		int counts;

		start = MPI_Wtime();

		MPI_Reduce(&count, &counts, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		double pi = ((double)counts / (double)NUM_ITER) * 4.0;

		stop = MPI_Wtime();
		diff = stop - start;

		printf("pi = %f\n", pi);
		printf("time: %fs\n", diff);
	}
	else
		MPI_Reduce(&count, NULL, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Finalize();
	
	return 0;
}
