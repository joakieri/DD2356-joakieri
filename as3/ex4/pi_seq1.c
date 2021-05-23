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

	int count = 0;
	double x, y, z;

	srand(SEED * rank);

	double start, stop, diff;
	if (rank == 0) start = MPI_Wtime();

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
		MPI_Status status;
		int other_count;

		for (int i = 1; i < size; i++) {
			MPI_Recv(&other_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			count += other_count;
		}
		double pi = ((double)count / (double)NUM_ITER) * 4.0;

		stop = MPI_Wtime();		
		diff = stop - start;

		printf("pi = %f\n", pi);
		printf("time: %fs\n", diff);
	}
	else
		MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

	MPI_Finalize();
	
	return 0;
}
