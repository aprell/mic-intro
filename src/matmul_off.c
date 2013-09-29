#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#define DIM 4 // Matrix is DIM x DIM

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

static void mm(double *A, double *B, double *C)
{
	int i, j, k;

	#pragma offload target(mic) \
		in(A, B : length(DIM * DIM)) \
		inout(C : length(DIM * DIM))

	#pragma omp parallel for schedule(static)
	for (i = 0; i < DIM; i++) {
		for (k = 0; k < DIM; k++) {
			for (j = 0; j < DIM; j++) {
				C(i,j) += A(i,k) * B(k,j);
			}
		}
	}
}

int main(void)
{
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
			C(i,j) = 0.0;
		}
	}

	print_matrix(A);
	print_matrix(B);
	mm(A, B, C);
	print_matrix(C);

	free(A);
	free(B);
	free(C);

	return 0;
}
