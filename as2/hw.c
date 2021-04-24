#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) {

	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		printf("Hello World from Thread %d!\n", id);
	}

	return 0;
}
