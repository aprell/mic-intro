#include <stdio.h>

int main(void)
{
	#pragma offload target(mic)
#ifdef __MIC__
	printf("Hello from MIC!\n");
#else
	printf("Hello from Host!\n");
#endif

	return 0;
}
