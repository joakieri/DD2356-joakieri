#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]){

    int rank, size, i, provided;
    
    // number of cells (global)
    int nxc = 128; // make sure nxc is divisible by size
    double L = 2*3.1415; // Length of the domain
    MPI_Status status;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // number of nodes (local to the process): 0 and nxn_loc-1 are ghost cells 
    int nxn_loc = nxc / size + 3; // number of nodes is number cells + 1; we add also 2 ghost cells
    double L_loc = L / ((double) size);
    double dx = L / ((double) nxc);
    
    // define out function
    double *f = calloc(nxn_loc, sizeof(double)); // allocate and fill with z
    double *dfdx = calloc(nxn_loc, sizeof(double)); // allocate and fill with z

    for (i=1; i<(nxn_loc-1); i++)
      f[i] = sin(L_loc*rank + (i-1) * dx);
    
    // need to communicate and fill ghost cells f[0] and f[nxn_loc-1]
    // communicate ghost cells
    if (rank == 0) {
        MPI_Send(&f[nxn_loc-3], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&f[0], 1, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&f[2], 1, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&f[nxn_loc-1], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
    }
    else {
        MPI_Recv(&f[0], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &status);
       	MPI_Send(&f[nxn_loc-3], 1, MPI_DOUBLE, (rank + 1) % size, 0, MPI_COMM_WORLD);
        MPI_Recv(&f[nxn_loc-1], 1, MPI_DOUBLE, (rank + 1) % size, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&f[2], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
    }

    // here we finish the calculations

    // calculate first order derivative using central difference
    // here we need to correct value of the ghost cells!
    for (i=1; i<(nxn_loc-1); i++)
      dfdx[i] = (f[i+1] - f[i-1])/(2*dx);

    
    // Print f values
    if (rank==0){ // print only rank 0 for convenience
        printf("My rank %d of %d\n", rank, size );
        printf("Here are my values for f including ghost cells\n");
        printf("%f (ghost)\n", f[0]);
        for (i=1; i<(nxn_loc-1); i++)
	       printf("%f\n", f[i]);
        printf("%f (ghost)\n", f[nxn_loc-1]);
        
        double x1 = L_loc*rank,
               x2 = L_loc*rank + (nxn_loc-3)*dx,
               dfdx1 = dfdx[1],
               dfdx2 = dfdx[nxn_loc-2];
        printf("Error of derivates at end points:\n");
        printf("|cos(%f) - f'(%f)| = %f\n", x1, x1, fabs(cos(x1) - dfdx1));
        printf("|cos(%f) - f'(%f)| = %f\n", x2, x2, fabs(cos(x2) - dfdx2));
    }   

    MPI_Finalize();
}






