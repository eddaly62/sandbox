// etime.c
// measure elapse time function

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

enum ETIME {START = 0, END = 1};

struct timeval start, end;


// Determine elapse time
double elapsed_time(enum ETIME sts, struct timeval *start, struct timeval *end)
{
	long long startusec = 0, endusec = 0;
	double elapsed = 0;

	if (sts == START)
	{
		gettimeofday(start, NULL);
	}
	else
	{
		gettimeofday(end, NULL);
		startusec = start->tv_sec * 1000000 + start->tv_usec;
		endusec = end->tv_sec * 1000000 + end->tv_usec;
		elapsed = (double)(endusec - startusec) / 1000000.0;	// seconds
		elapsed = (double)(endusec - startusec);				// usec
	}
	return elapsed;
}


int main(int argc, char *argv[])
{
	double elapsedt=0;

	elapsedt = elapsed_time(START, &start, &end);
	sleep(1);
	elapsedt = elapsed_time(END, &start, &end);
	
	fprintf(stdout, "Elapsed time is %f seconds\n", elapsedt);

	return 0;

}



