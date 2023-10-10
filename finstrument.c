// finstrument.c
// test of using finstrument compile option to profile code

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

enum ETIME {START = 0, END = 1};

//#if 0
#define MAX_LEN	64
typedef struct fc {
	int call_count;
	void *this_fn;
	char sfile[MAX_LEN];
	int line;
	struct timeval start;
	struct timeval end;
	double time_max;
	double time_min;
} STATS;

int stats_count;
STATS pstats[100];


void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void update_stats_start(char *f, int line, void *this_fn) __attribute__((no_instrument_function));
void update_stats_end(char *f, int line, void *this_fn) __attribute__((no_instrument_function));


void update_stats_start(char *f, int line, void *this_fn){

	int i;

	for (i = 0; i < 100; i++) {
		if (this_fn == pstats[i].this_fn) {
			// match
			pstats[i].call_count++;
			return;
		}
	}

	// not in table
	pstats[stats_count].call_count = 1;
	pstats[stats_count].line = line;
	pstats[stats_count].this_fn = this_fn;
	memcpy(pstats[stats_count].sfile, f, strlen(f));
	gettimeofday(&pstats[stats_count].start, NULL);
	stats_count++;
	return;
}

void update_stats_end(char *f, int line, void *this_fn){

	int i;
	double startusec, endusec, elapsed;

	for (i = 0; i < 100; i++) {
		if (this_fn == pstats[i].this_fn) {
			// match
			gettimeofday(&pstats[i].end, NULL);
			startusec = pstats[i].start.tv_sec * 1000000 + pstats[i].start.tv_usec;
			endusec = pstats[i].end.tv_sec * 1000000 + pstats[i].end.tv_usec;
			elapsed = (endusec - startusec);	// usec
			pstats[i].time_max = elapsed;
			pstats[i].time_min = elapsed;
			return;
		}
	}

	// not in table
	fprintf(stderr, "table insertion error\n");
	return;
}

void __cyg_profile_func_enter(void *this_fn, void *call_site) {
	update_stats_start(__FILE__, __LINE__, this_fn);
	//printf("--> File(%s) Line(%d) Count(%d) %p %p\n", __FILE__, __LINE__, cnt, this_fn, call_site);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site) {
	//printf("<-- %p %p\n", this_fn, call_site);
	update_stats_end(__FILE__, __LINE__, this_fn);
}
//#endif

struct timeval start, end;

void do_nothing1(void) {
	printf("do nothing 1\n");
}

void do_nothing2(void) {
	printf("do nothing 2\n");
}


// Determine elapse time
double elapsed_time(enum ETIME sts, struct timeval *start, struct timeval *end)
{
	long long startusec = 0, endusec = 0;
	double elapsed = 0;

	do_nothing1();

	do_nothing2();

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

	fprintf(stdout, "Elapsed time is %f usec\n", elapsedt);

	return 0;

}



