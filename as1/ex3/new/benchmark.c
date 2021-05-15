#ifndef N
  #define N 5000
#endif
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

//#define FREQ 2.2e9
#define FREQ 3.4e9

// Use the preprocessor so we know definitively that these are placed inline
#define RDTSC_START()            \
 	__asm__ volatile("CPUID\n\t" \
	                 "RDTSC\n\t" \
       	                 "mov %%edx, %0\n\t" \
       	                 "mov %%eax, %1\n\t" \
       	                 : "=r" (start_hi), "=r" (start_lo) \
       	                 :: "%rax", "%rbx", "%rcx", "%rdx");

#define RDTSC_STOP()              \
       	__asm__ volatile("RDTSCP\n\t" \
 		         "mov %%edx, %0\n\t" \
 		         "mov %%eax, %1\n\t" \
 		         "CPUID\n\t" \
       	                 : "=r" (end_hi), "=r" (end_lo) \
 		         :: "%rax", "%rbx", "%rcx", "%rdx");


// Returns the elapsed time given the high and low bits of the start and stop time.
uint64_t elapsed(uint32_t start_hi, uint32_t start_lo, uint32_t end_hi, uint32_t end_lo)
{
	uint64_t start = (((uint64_t)start_hi) << 32) | start_lo;
	uint64_t end   = (((uint64_t)end_hi)   << 32) | end_lo;
	return end-start;
}

static uint32_t start_hi=0, start_lo=0; 
static uint32_t   end_hi=0,   end_lo=0;

double mysecond();

int main(){
  int i, j;
  uint64_t ticks; // timers                                                         
  double a[N], b[N], c[N]; // arrays  
  double sum;

  // init arrays                                                                   
  for (i = 0; i < N; i++){
    a[i] = 47.0;
    b[i] = 3.1415;
  }

  // measure performance
  for(i = -1; i < N; i++) {
    if (i == -1) {
      RDTSC_START();
      continue;
    }
    c[i] = a[i]*b[i];
  }
  RDTSC_STOP();
  ticks = elapsed(start_hi, start_lo, end_hi, end_lo);

  sum = 0;
  for (i = 0; i < N; i++)
    sum += c[i];

  printf("Execution time: %11.8f s\n", ticks / FREQ);
  printf("%f (ignore)\n", sum);
  return 0;
}

// function with timer                                                             
double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}
