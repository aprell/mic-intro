#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 30

#ifdef __INTEL_OFFLOAD
__attribute__((target(mic)))
#endif
static unsigned long long fib(unsigned long long n)
{
	unsigned long long x, y;

	if (n < 2)
		return n;

	#pragma omp task shared(x)
	x = fib(n-1);

	#pragma omp task shared(y)
	y = fib(n-2);

	#pragma omp taskwait

	return x + y;
}

int main(void)
{
	unsigned long long n;

	#pragma offload target(mic)
	#pragma omp parallel
	{
		#pragma omp single
		{
			printf("Running %d threads\n", omp_get_num_threads());
			fflush(0);
			n = fib(N);
		}
	}

	printf("%llu\n", n);

	return 0;
}
