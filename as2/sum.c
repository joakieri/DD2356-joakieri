#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define N 10000000
#define RUNS 5
#define MAX_THREADS 32

double arr[N], run_time = 0;
int threads_used = 1;

void generate_random(double *input, size_t size) {
	for (size_t i = 0; i < size; i++) {
		input[i] = rand() / (double)(RAND_MAX);
	}
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
	
	printf("\tavg = %f, stdev = %f\n", avg, stdev);
}

double serial_sum(double *x, size_t size) {
	double sum_val = 0.0;
	
	run_time = omp_get_wtime();
	for (size_t i = 0; i < size; i++) {
		sum_val += x[i];
	}
	run_time = omp_get_wtime() - run_time;

 	return sum_val;
}

double omp_sum(double *x, size_t size) {
	double sum_val = 0.0;
	
	run_time = omp_get_wtime();
	#pragma omp parallel for
	for (int i = 0; i < size; i++) {
		sum_val += x[i];
	}
	run_time = omp_get_wtime() - run_time;

 	return sum_val;
}

double omp_critical_sum(double *x, size_t size) {
	double sum_val = 0.0;
	
	run_time = omp_get_wtime();
	#pragma omp parallel for
	for (int i = 0; i < size; i++) {
		#pragma omp critical
		{
			sum_val += x[i];
		}
	}
	run_time = omp_get_wtime() - run_time;

 	return sum_val;
}

double omp_local_sum(double *x, size_t size) {
	double sum_val = 0.0;
	double local_sum[MAX_THREADS];
	
	for (int i = 0; i < threads_used; i++)
		local_sum[i] = 0.0;
	run_time = omp_get_wtime();
	#pragma omp parallel shared(local_sum)
	{
		int num_threads = omp_get_num_threads();
		int id = omp_get_thread_num();
		local_sum[id] = 0.0;
		for (int i = id; i < size; i += num_threads) {
			local_sum[id] += x[i];
		}
	}
	for (int i = 0; i < threads_used; i++)
		sum_val += local_sum[i];
	run_time = omp_get_wtime() - run_time;

 	return sum_val;
}

typedef struct { double val; char pad[128];} data;

double omp_local_sum2(double *x, size_t size) {
	double sum_val = 0.0;
	data local_sum[MAX_THREADS];

	for (int i = 0; i < threads_used; i++)
		local_sum[i].val = 0.0;
	run_time = omp_get_wtime();
	#pragma omp parallel shared(local_sum)
	{
		int num_threads = omp_get_num_threads();
		int id = omp_get_thread_num();
		local_sum[id].val = 0.0;
		for (int i = id; i < size; i += num_threads) {
			local_sum[id].val += x[i];
		}
	}
	for (int i = 0; i < threads_used; i++)
		sum_val += local_sum[i].val;
	run_time = omp_get_wtime() - run_time;

 	return sum_val;
}

int main() {
	int threads[9] = {1, 2, 4, 8, 16, 20, 24, 28, 32};
	double times[RUNS], sums[RUNS];

	srand(time(NULL));
	generate_random(arr, N);
	
	printf("Test of serial_sum:\n");
	for (int i = 0; i < RUNS; i++) {
		serial_sum(arr, N);
		times[i] = run_time;
	}
	print_avg_stdev(times, RUNS);

	printf("Test of omp_sum:\n");
	printf("\tThreads = 32\n");
	omp_set_num_threads(32);
	for (int i = 0; i < RUNS; i++) {
		sums[i] = omp_sum(arr, N);
		times[i] = run_time;
	}
	print_avg_stdev(times, RUNS);
	for (int i = 1; i < RUNS; i++)
		if (sums[i-1] != sums[i]) {
			printf("\tNote: omp_sum does not give a correct result.\n");
			break;
		}

	printf("Test of omp_sum and omp_critical_sum:\n");
	for (int j = 0; j < 9; j++) {
		printf("\tThreads = %d\n", threads[j]);
		omp_set_num_threads(threads[j]);
		for (int i = 0; i < RUNS; i++) {
			omp_sum(arr, N);
			times[i] = run_time;
		}
		printf("\tomp_sum:\t");
		print_avg_stdev(times, RUNS);
		for (int i = 0; i < RUNS; i++) {
			omp_critical_sum(arr, N);
			times[i] = run_time;
		}
		printf("\tomp_critical_sum:");
		print_avg_stdev(times, RUNS);
	}

	printf("Test of omp_local_sum:\n");
	for (int j = 0; j < 9; j++) {
		printf("\tThreads = %d\n", threads[j]);
		omp_set_num_threads(threads[j]);
		threads_used = threads[j];
		for (int i = 0; i < RUNS; i++) {
			omp_local_sum(arr, N);
			times[i] = run_time;
		}
		print_avg_stdev(times, RUNS);
	}

	printf("Test of omp_local_sum2:\n");
	for (int j = 0; j < 9; j++) {
		printf("\tThreads = %d\n", threads[j]);
		omp_set_num_threads(threads[j]);
		threads_used = threads[j];
		for (int i = 0; i < RUNS; i++) {
			omp_local_sum2(arr, N);
			times[i] = run_time;
		}
		print_avg_stdev(times, RUNS);
	}

	return 0;
}
