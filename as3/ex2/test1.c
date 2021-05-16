#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]){

    int rank, size, i, provided;
    MPI_Status status;
    
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // number of nodes (local to the process): 0 and nxn_loc-1 are ghost cells 
    int nxn_loc = 3;
    
    // define out function
    int *f = calloc(nxn_loc, sizeof(int));
    f[1] = rank;

    if (rank == 0) {
        MPI_Send(&f[1], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&f[0], 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&f[1], 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&f[2], 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
    }
    else {
        MPI_Recv(&f[0], 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
       	MPI_Send(&f[1], 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
        MPI_Recv(&f[2], 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&f[1], 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
    }

    printf("Rank %d, Values: %d %d %d\n", rank, f[0], f[1], f[2]);

    MPI_Finalize();
}






