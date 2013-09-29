#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#include "wtime.h"

#define DIM 2048 // Matrix is DIM x DIM
#define N 10 // Number of repetitions

static double **A, **B, **C;

static void zero_product(double **C)
{
	int i, j;

	#pragma omp for schedule(static)
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			C[i][j] = 0.0;
		}
	}
}

static void mm(double **A, double **B, double **C)
{
	int i, j, k;

	zero_product(C);

	#pragma omp for schedule(static)
	// Innermost loop is a candidate for vectorization
	for (i = 0; i < DIM; i++) {
		for (k = 0; k < DIM; k++) {
			for (j = 0; j < DIM; j++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

int main(void)
{
	double start, elapsed;
	double nflop;
	int i, j;

	// Allocate matrices
	A = (double **)malloc(DIM * sizeof(double *));
	B = (double **)malloc(DIM * sizeof(double *));
	C = (double **)malloc(DIM * sizeof(double *));
	assert(A && B && C);

	for (i = 0; i < DIM; i++) {
		A[i] = (double *)malloc(DIM * sizeof(double));
		B[i] = (double *)malloc(DIM * sizeof(double));
		C[i] = (double *)malloc(DIM * sizeof(double));
		assert(A[i] && B[i] && C[i]);
	}

	srand(42);

	// Setup matrices A and B with random values
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			A[i][j] = (double)rand() / (double)RAND_MAX;
			B[i][j] = (double)rand() / (double)RAND_MAX;
			C[i][j] = 0.0;
		}
	}

	// Warm-up
	#pragma omp parallel
	{
		mm(A, B, C);
		zero_product(C);
	}

	start = Wtime_sec();

	for (i = 0; i < N; i++) {
		#pragma omp parallel
		mm(A, B, C);
	}

	elapsed = Wtime_sec() - start;

	nflop = N * 2 * (double)DIM * (double)DIM * (double)DIM;

	printf("Average execution time: %.2lfs\n", elapsed/N);
	printf("Average GFLOPS: %.2lf\n", nflop/(elapsed * 1e9));

	for (i = 0; i < DIM; i++) {
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}
	
	free(A);
	free(B);
	free(C);

	return 0;
}
