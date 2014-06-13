/*
 * The following code is based on the example program in chapter 2, pages
 * 35-37, of Intel(R) Xeon Phi(TM) Coprocessor High-Performance Programming,
 * written by Jim Jeffers and James Reinders.
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "wtime.h"

#define ARRAY_SIZE (1024 * 1024)
#define ITERS_OUTER 48000000L
#define ITERS_INNER 64

typedef float real;

static real A[ARRAY_SIZE] __attribute__((align(64)));
static real B[ARRAY_SIZE] __attribute__((align(64)));

int main(void)
{
	double start, elapsed;
	double gflops;
	real a = 1.1;
	int i, k, N;
	long j;

	// Initialize
	#pragma omp parallel for
	for (i = 0; i < ARRAY_SIZE; i++) {
		A[i] = i + (real)0.1;
		B[i] = i + (real)0.2;
	}

	#pragma omp parallel
	{
		#pragma omp single
		{
			N = omp_get_num_threads();
			start = Wtime_sec();
		} // Barrier

		#pragma omp for private(j, k)
		for (i = 0; i < N; i++) {
			int offset = i * ITERS_INNER;
			for (j = 0; j < ITERS_OUTER; j++) {
				//#pragma simd
				for (k = 0; k < ITERS_INNER; k++) {
					A[k+offset] = a * A[k+offset] + B[k+offset];
				}
			}
		}

		#pragma omp single
		{
			elapsed = Wtime_sec() - start;
		} // Barrier

	} // End of parallel region

	gflops = (2 * ITERS_INNER * ITERS_OUTER * N) / (elapsed * 1e9);

	printf("Execution time: %.2lfs\n", elapsed);
	printf("GFLOPS: %.2lf\n", gflops);

	return 0;
}
