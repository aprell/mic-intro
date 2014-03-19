#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mkl.h>
#include "wtime.h"

#define DIM 2048 // Matrix is DIM x DIM
#define N 10 // Number of repetitions

#define A(i,j) A[i*DIM+j]
#define B(i,j) B[i*DIM+j]
#define C(i,j) C[i*DIM+j]

static double *A, *B, *C;

static void print_matrix(double *C)
{
	int i, j;

	printf("\n%p\n", C);

	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			printf("%5.2lf ", C(i,j));
		}
		printf("\n");
	}
}

void mm(double *A, double *B, double *C)
{
	double alpha = 1.0, beta = 0.0;
	int dim = DIM;

	// When beta is equal to zero, then c need not be set on input
	dgemm("N", "N", &dim, &dim, &dim, &alpha, B, &dim, A, &dim, &beta, C, &dim);
}

int main(void)
{
	double start, elapsed;
	double nflop;
	int i, j;

	// Allocate matrices
	A = (double *)malloc(DIM * DIM * sizeof(double));
	B = (double *)malloc(DIM * DIM * sizeof(double));
	C = (double *)malloc(DIM * DIM * sizeof(double));
	assert(A && B && C);

	// Setup matrices A, B, and C
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			A(i,j) = i * DIM + j + 1;
			B(i,j) = j >= i ? 1 : 0;
		}
	}

	// Warm-up
	mm(A, B, C);

	start = Wtime_sec();

	for (i = 0; i < N; i++) {
		mm(A, B, C);
	}

	elapsed = Wtime_sec() - start;

	nflop = N * 2 * (double)DIM * (double)DIM * (double)DIM;

	printf("Average execution time: %.2lfs\n", elapsed/N);
	printf("Average GFLOPS: %.2lf\n", nflop/(elapsed * 1e9));

	//print_matrix(C);

	free(A);
	free(B);
	free(C);

	return 0;
}
