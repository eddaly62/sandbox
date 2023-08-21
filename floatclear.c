// floatclear.c

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	float test;

	memset(&test, 0x0, sizeof(float));

	printf("test = %f\n", test);

	return 0;
}



