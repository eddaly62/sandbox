// finstrument.c
// test of using finstrument compile option to profile code

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

enum ETIME {START = 0, END = 1};

//#if 0
#define MAX_LEN	64
typedef struct st {
	int call_count;
	void *this_fn;
	char sfile[MAX_LEN];
	int line;
	struct timeval start;
	struct timeval end;
	unsigned long time_start;
	unsigned long time_end;
	unsigned long time_max;
	unsigned long time_min;
} STATS;

typedef struct ps {
	int stats_count;
	STATS stats[100];
} PSTATS;

PSTATS pstats;

void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void update_stats_start(char *f, int line, void *this_fn) __attribute__((no_instrument_function));
void update_stats_end(char *f, int line, void *this_fn) __attribute__((no_instrument_function));
void print_stats(void) __attribute__((no_instrument_function));

void print_stats(void) {

	int i;
	for (i = 0; i < pstats.stats_count; i++) {
		printf("file(%s), this_fn(%p), count(%d), min time(%lu)\n",
		 pstats.stats[i].sfile, pstats.stats[i].this_fn, pstats.stats[i].call_count, pstats.stats[i].time_start);
	}
}

void update_stats_start(char *f, int line, void *this_fn){

	int i;
	unsigned long startusec;

#if 0
	for (i = 0; i <= 100; i++) {
		if (this_fn == pstats.stats[i].this_fn) {
			// match
			pstats.stats[i].call_count++;
			return;
		}
	}
#endif
	// not in table
	i = pstats.stats_count;
	pstats.stats[i].call_count = 1;
	pstats.stats[i].line = line;
	pstats.stats[i].this_fn = this_fn;
	memcpy(pstats.stats[i].sfile, f, strlen(f));
	gettimeofday(&pstats.stats[i].start, NULL);
	startusec = (pstats.stats[i].start.tv_sec * 1000000) + pstats.stats[i].start.tv_usec;
	pstats.stats[i].time_start = startusec;
	if (pstats.stats_count < 100) {
		pstats.stats_count++;
	}
	return;
}

void update_stats_end(char *f, int line, void *this_fn){

	int i;
	long long startusec, endusec;
	double elapsed;
#if 0
	for (i = 0; i <= 100; i++) {
		if (this_fn == pstats.stats[i].this_fn) {
			// match
			gettimeofday(&pstats.stats[i].end, NULL);
			startusec = (pstats.stats[i].start.tv_sec * 1000000) + pstats.stats[i].start.tv_usec;
			endusec = (pstats.stats[i].end.tv_sec * 1000000) + pstats.stats[i].end.tv_usec;
			elapsed = (double)(endusec - startusec);	// usec
			pstats.stats[i].time_max = elapsed;
			pstats.stats[i].time_min = elapsed;
			return;
		}
	}
	// not in table
	fprintf(stderr, "table insertion error\n");
#endif
	return;
}

void __cyg_profile_func_enter(void *this_fn, void *call_site) {
	update_stats_start(__FILE__, __LINE__, this_fn);
	printf("--> File(%s) Line(%d) Count(%d) %p %p\n", __FILE__, __LINE__, pstats.stats_count, this_fn, call_site);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site) {
	update_stats_end(__FILE__, __LINE__, this_fn);
	printf("--> File(%s) Line(%d) Count(%d) %p %p\n", __FILE__, __LINE__, pstats.stats_count, this_fn, call_site);
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
	pstats.stats_count = 0;

	elapsedt = elapsed_time(START, &start, &end);
	sleep(1);
	elapsedt = elapsed_time(END, &start, &end);

	fprintf(stdout, "Elapsed time is %f usec\n", elapsedt);

	print_stats();
	return 0;

}



