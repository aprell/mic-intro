#include <stdio.h>

int main(void)
{
	int num_devs = 0, i;

#ifdef __INTEL_OFFLOAD
	num_devs = _Offload_number_of_devices();
	printf("Number of MIC coprocessors installed: %d\n", num_devs);
#endif

	for (i = 0; i < num_devs; i++) {
		#pragma offload target(mic:i)
		printf("Hello from MIC %d!\n", i);
	}

	return 0;
}
