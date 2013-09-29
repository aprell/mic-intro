#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#define N 1000 

int main(void)
{
	int sum = 0;
	int i;

	#pragma offload target(mic)
	#pragma omp parallel for reduction(+:sum)
	for (i = 1; i <= N; i++) {
		sum += i;
	}

	printf("Sum is %d\n", sum);
	assert(sum == (N * (N + 1))/2);

	return 0;
}
