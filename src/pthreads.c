#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

#ifndef min
#define min(a, b) ((a) > (b) ? (b) : (a))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif


#ifdef __INTEL_OFFLOAD
__attribute__((target(mic)))
#endif
static void set_thread_affinity(pthread_t t, int cpu)
{
	cpu_set_t cpuset;

	CPU_ZERO(&cpuset);
	CPU_SET(cpu, &cpuset);
	pthread_setaffinity_np(t, sizeof(cpu_set_t), &cpuset);
}

#ifdef __INTEL_OFFLOAD
__attribute__((target(mic)))
#endif
static void print_thread_affinity(pthread_t t, int ID)
{
	cpu_set_t cpuset;
	int i;

	pthread_getaffinity_np(t, sizeof(cpu_set_t), &cpuset);

	for (i = 0; i < CPU_SETSIZE; i++) {
		if (CPU_ISSET(i, &cpuset))
			printf("Thread %5d running on proc %5d\n", ID, i);
	}
}

#ifdef __INTEL_OFFLOAD
__attribute__((target(mic)))
#endif
static int cpu_count(void)
{
	cpu_set_t cpuset;
	
	pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

	return CPU_COUNT(&cpuset);
}

#ifdef __INTEL_OFFLOAD
__attribute__((target(mic)))
#endif
static void *thread_func(void *args)
{
	int ID = *(int *)args;

	print_thread_affinity(pthread_self(), ID);

	return NULL;
}

int main(int argc, char *argv[])
{
	int num_threads, num_cpus, i;
	cpu_set_t cpuset;

	if (argc == 2) {
		num_threads = max(atoi(argv[1]), 1);
	} else {
		num_threads = 1;
	}

	#pragma offload target(mic)
	{
		int IDs[num_threads];
		pthread_t threads[num_threads];

		num_cpus = cpu_count();
		printf("Number of logical processors: %d\n", num_cpus);

		set_thread_affinity(pthread_self(), 0);

		for (i = 1; i < num_threads; i++) {
			IDs[i] = i;
			pthread_create(&threads[i], NULL, thread_func, &IDs[i]);
			set_thread_affinity(threads[i], i % num_cpus);
		}

		IDs[0] = 0;
		thread_func(&IDs[0]);

		for (i = 1; i < num_threads; i++) {
			pthread_join(threads[i], NULL);
		}
	} // end offload

	return 0;
}
