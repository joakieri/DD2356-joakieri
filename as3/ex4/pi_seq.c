#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <mpi.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[]){
	
	MPI_Init(&argc, &argv);
	
	int count = 0;
	double x, y, z, pi;

	srand(SEED); // Important: Multiply SEED by "rank" when you introduce MPI!

	double start, stop, diff;
	start = MPI_Wtime();

	// Calculate PI following a Monte Carlo method
	for (int iter = 0; iter < NUM_ITER; iter++) {
		// Generate random (X,Y) points
		x = (double)random() / (double)RAND_MAX;
		y = (double)random() / (double)RAND_MAX;
		//z = sqrt((x*x) + (y*y));
		z = x*x + y*y;

		// Check if point is in unit circle
		if (z <= 1.0) {
			count++;

		}
	}

	// Estimate Pi and display the result
	pi = ((double)count / (double)NUM_ITER) * 4.0;
	
	stop = MPI_Wtime();
	diff = stop - start;

	printf("The result is %f\n", pi);
	printf("Time: %f\n", diff);

	MPI_Finalize();

	return 0;
}
