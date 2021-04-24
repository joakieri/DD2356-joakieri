    // the code calculates a DFT of a random complex number input and 
    // then an IDFT. The IDFT result should be the input vector 
    // to compile with gcc
    // gcc -Wall -O2 -fopenmp -o DFTW DFTW.c 
    // written by stef

    // exercise


	#include "stdio.h" // printf
	#include "stdlib.h" // malloc and rand for instance. Rand not thread safe!
	#include "time.h"   // time(0) to get random seed
	#include "math.h"  // sine and cosine
	#include "omp.h"   // openmp library like timing

	
	// two pi
	#define PI2 6.28318530718
	// this for the rounding error, increasing N rounding error increases
    // 0.01 precision good for N > 8000
	#define R_ERROR 0.01
	#define RUNS 5
	
	typedef struct {double val; char pad[128];} data;

	// main routine to calculate DFT
	int DFT(int idft, data* xr, data* xi, data* Xr_o, data* Xi_o, int N);
	// set the input array with random number
	int fillInput(data* xr, data* xi, int N);
	// set to zero the input vector
	int setOutputZero(data* Xr_o, data* Xi_o, int N);	
	// check if x = IDFT(DFT(x))
	int checkResults(data* xr, data* xi, data* xr_check, data* xi_check, data* Xr_o, data* Xi_r, int N);
	// print the results of the DFT
	int printResults(data* xr, data* xi, int N);

	double run_times[RUNS];
	void print_avg_stdev(double *x, size_t size);

	int main(int argc, char* argv[]){
	  // size of input array
	  const int N = 10000; // 8,000 is a good number for testing
	  printf("DFTW calculation with N = %d \n",N);
	  
	  // Allocate array for input vector
	  data* xr = (data*) malloc (N *sizeof(data));
	  data* xi = (data*) malloc (N *sizeof(data));
	  fillInput(xr,xi,N);

	  // for checking purposes
	  data* xr_check = (data*) malloc (N *sizeof(data));
	  data* xi_check = (data*) malloc (N *sizeof(data));

	  
	  // Allocate array for output vector
	  data* Xr_o = (data*) malloc (N *sizeof(data));
	  data* Xi_o = (data*) malloc (N *sizeof(data));
     
     	  printf("Threads = %d\n", 32);
	  omp_set_num_threads(32);
     	  for (int j = 0; j < RUNS; j++) {
     	  	  printf("Run #%d\n", j+1);
     	  	  setOutputZero(xr_check,xi_check,N);
     	  	  setOutputZero(Xr_o,Xi_o,N);
		  
		  // start timer
		  double start_time = omp_get_wtime();
		  

		  // DFT
		  int idft = 1;
		  DFT(idft,xr,xi,Xr_o,Xi_o,N);
		  // IDFT
		  idft = -1;
		  DFT(idft,Xr_o,Xi_o,xr_check,xi_check,N);
		  

		  // stop timer
		  double run_time = omp_get_wtime() - start_time;
		  run_times[j] = run_time;
		  printf("DFTW computation in %f seconds\n",run_time);
		  
		  // check the results: easy to make correctness errors with openMP
		  checkResults(xr,xi,xr_check,xi_check,Xr_o, Xi_o, N);
		  // print the results of the DFT
	      	  #ifdef DEBUG
	      	  printResults(Xr_o,Xi_o,N);
		  #endif
	  }
	  print_avg_stdev(run_times, RUNS);
	  
	  // take out the garbage
	  free(xr); free(xi);
	  free(Xi_o); free(Xr_o);
	  free(xr_check); free(xi_check);

	  return 1;
	}

void print_avg_stdev(double *x, size_t size) {
	double avg = 0.0;
	double stdev = 0.0;
	double sum = 0.0;

	for (int i = 0; i < size; i++)
		avg += x[i];
	avg /= (double)size;
	
	for (int i = 0; i < size; i++)
		stdev += pow(x[i] - avg, 2);
	stdev /= (double)size;
	stdev = sqrt(stdev);
	
	printf("avg = %f, stdev = %f\n", avg, stdev);
}

	// DFT/IDFT routine
	// idft: 1 direct DFT, -1 inverse IDFT (Inverse DFT)
	int DFT(int idft, data* xr, data* xi, data* Xr_o, data* Xi_o, int N){
		#pragma omp parallel shared(xr, xi, Xr_o, Xi_o)
		{
		int threads = omp_get_num_threads();
		int id = omp_get_thread_num();
		for (int k=id ; k<N ; k += threads) {
	        	for (int n=0 ; n<N ; n++)  {
	        		// Real part of X[k]
	            		Xr_o[k].val += xr[n].val * cos(n * k * PI2 / N) + idft*xi[n].val*sin(n * k * PI2 / N);
	            		// Imaginary part of X[k]
	      			Xi_o[k].val += -idft*xr[n].val * sin(n * k * PI2 / N) + xi[n].val * cos(n * k * PI2 / N);
	        	} 
	    	}
		
	    	// normalize if you are doing IDFT
	    	if (idft==-1){
	    		for (int n=id ; n<N ; n+=threads){
	    			Xr_o[n].val /=N;
	    			Xi_o[n].val /=N;
	    		}
	    	}
		}
	  	return 1; 
	}


	
	// set the initial signal 
    // be careful with this 
    // rand() is NOT thread safe in case
	int fillInput(data* xr, data* xi, int N){
	    srand(time(0));
	    for(int n=0; n < 100000;n++) // get some random number first 
	    	rand();
	    for(int n=0; n < N;n++){
	       // Generate random discrete-time signal x in range (-1,+1)
	       //xr[n] = ((double)(2.0 * rand()) / RAND_MAX) - 1.0;
	       //xi[n] = ((double)(2.0 * rand()) / RAND_MAX) - 1.0;
	       // constant real signal
	       xr[n].val = 1.0;
	       xi[n].val = 0.0;
	    }
		return 1; 
	}

	// set to zero the output vector
	int setOutputZero(data* Xr_o, data* Xi_o, int N){
	for(int n=0; n < N;n++){
	       Xr_o[n].val = 0.0;
	       Xi_o[n].val = 0.0; 
	    }
		return 1;
	}

	// check if x = IDFT(DFT(x))
	int checkResults(data* xr, data* xi, data* xr_check, data* xi_check, data* Xr_o, data* Xi_r, int N){
		// x[0] and x[1] have typical rounding error problem
		// interesting there might be a theorem on this
		for(int n=0; n < N;n++){
			if (fabs(xr[n].val - xr_check[n].val) > R_ERROR)
			    printf("ERROR - x[%d] = %f, inv(X)[%d]=%f \n",n,xr[n].val, n,xr_check[n].val);
			if (fabs(xi[n].val - xi_check[n].val) > R_ERROR)
			    printf("ERROR - x[%d] = %f, inv(X)[%d]=%f \n",n,xi[n].val, n,xi_check[n].val);

		}
		printf("Xre[0] = %f \n",Xr_o[0].val);
		return 1;
	}

	// print the results of the DFT
	int printResults(data* xr, data* xi, int N){
		for(int n=0; n < N;n++)
			    printf("Xre[%d] = %f, Xim[%d] = %f \n", n, xr[n].val, n, xi[n].val);
		return 1;
	}
