// finstrument.c
// test of using finstrument compile option to profile code

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <dlfcn.h>



//#if 0
#define MAX_LEN	64
#define MAX_FUNCTIONS	4096

Dl_info info;
void *extra_info;

typedef struct st {
	void *this_fn;
	void *call_site;
	char sfile[MAX_LEN];
	unsigned long call_count;
	unsigned long time_start;
	unsigned long time_end;
	unsigned long time_max;
	unsigned long time_min;
} STATS;

typedef struct ps {
	int stats_count;
	STATS stats[MAX_FUNCTIONS];
} PSTATS;

PSTATS pstats = { .stats_count = 0 };

void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void update_stats_start(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void update_stats_end(void *this_fn, void *call_site) __attribute__((no_instrument_function));
void print_stats(void) __attribute__((no_instrument_function));
void fprint_stats_csv(char *pathname) __attribute__((no_instrument_function));
void fprint_stats_md(char *pathname) __attribute__((no_instrument_function));
unsigned long get_time(void) __attribute__((no_instrument_function));
unsigned long get_time_diff(unsigned long start, unsigned long end) __attribute__((no_instrument_function));
int main(int argc, char *argv[]) __attribute__((no_instrument_function));
void sig_handler(int signum) __attribute__((no_instrument_function));
void fini(void) __attribute__((no_instrument_function));


unsigned long get_time(void) {

	struct timeval time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000000) + time.tv_usec);
}

unsigned long get_time_diff(unsigned long start, unsigned long end) {
	return (end - start);
}


void print_stats(void) {
	int i;
	for (i = 0; i < pstats.stats_count; i++) {
		dladdr(pstats.stats[i].this_fn, &info);
		strcpy(pstats.stats[i].sfile, info.dli_fname);

		printf("file(%s), this_fn(%p, %s), count(%lu), min time(%lu), max time(%lu)\n",
		 pstats.stats[i].sfile, pstats.stats[i].this_fn, info.dli_sname,
		 pstats.stats[i].call_count, pstats.stats[i].time_min, pstats.stats[i].time_max);
	}
}

// print results in csv file format
void fprint_stats_csv(char *pathname) {
	int i;
	FILE *fp;

	fp = fopen(pathname, "w");

	for (i = 0; i < pstats.stats_count; i++) {
		dladdr(pstats.stats[i].this_fn, &info);
		strcpy(pstats.stats[i].sfile, info.dli_fname);

		if (i == 0) {
			fprintf(fp, "file, function_ptr, function, count, min_time, max_time\n");
		}
		else {
			fprintf(fp, "%s, %p, %s, %lu, %lu, %lu\n",
			pstats.stats[i].sfile, pstats.stats[i].this_fn, info.dli_sname,
			pstats.stats[i].call_count, pstats.stats[i].time_min, pstats.stats[i].time_max);
		}
	}

	fclose(fp);
}

// print results in markdown file format
void fprint_stats_md(char *pathname) {
	int i;
	FILE *fp;

	fp = fopen(pathname, "w");

	for (i = 0; i < pstats.stats_count; i++) {
		dladdr(pstats.stats[i].this_fn, &info);
		strcpy(pstats.stats[i].sfile, info.dli_fname);

		if (i == 0) {
			//fprintf(fp, "| file | function_ptr | function | count | min_time | max_time |\n");
			fprintf(fp, "# %s\n\n", pathname);
			fprintf(fp, "| function | count | min_time (usec) | max_time (usec) |\n");
			fprintf(fp, "|----------|-------|-----------------|-----------------|\n");
		}
		else {
			//fprintf(fp, "| %s | %p | %s | %lu | %lu | %lu |\n",
			//pstats.stats[i].sfile, pstats.stats[i].this_fn, info.dli_sname,
			//pstats.stats[i].call_count, pstats.stats[i].time_min, pstats.stats[i].time_max);

			fprintf(fp, "| %s | %lu | %lu | %lu |\n",
			info.dli_sname, pstats.stats[i].call_count, pstats.stats[i].time_min, pstats.stats[i].time_max);
		}
	}

	fclose(fp);
}


void fini(void){
	print_stats();
	fprint_stats_csv("finstrument.csv");
	fprint_stats_md("finstrument.md");
}

void sig_handler(int signum) {
	//print_stats();
	exit(EXIT_SUCCESS);
}

void update_stats_start(void *this_fn, void *call_site){

	int i;
	unsigned long startusec;

	if (pstats.stats_count == 0) {
		signal(SIGINT, sig_handler);
		atexit(fini);
	}

	for (i = 0; i < MAX_FUNCTIONS; i++) {
		if (this_fn == pstats.stats[i].this_fn) {
			// match, already in table, just update the stuff that changed
			pstats.stats[i].call_count++;
			pstats.stats[i].time_start = get_time();
			return;
		}
	}

	// not in table
	i = pstats.stats_count;
	pstats.stats[i].call_count = 1;
	pstats.stats[i].this_fn = this_fn;
	pstats.stats[i].call_site = call_site;
	pstats.stats[i].time_start = get_time();
	pstats.stats[i].time_min = pstats.stats[i].time_start;
	pstats.stats[i].time_max = 0;
	if (pstats.stats_count < MAX_FUNCTIONS) {
		pstats.stats_count++;
	}
	return;
}

void update_stats_end(void *this_fn, void *call_site){

	int i;
	unsigned long elapsed;

	for (i = 0; i < MAX_FUNCTIONS; i++) {
		if (this_fn == pstats.stats[i].this_fn) {

			// match
			pstats.stats[i].time_end = get_time();
			elapsed = get_time_diff(pstats.stats[i].time_start, pstats.stats[i].time_end);

			if (elapsed < pstats.stats[i].time_min) {
				pstats.stats[i].time_min = elapsed;
			}

			if (elapsed > pstats.stats[i].time_max) {
				pstats.stats[i].time_max = elapsed;
			}
			return;
		}
	}

	// not in table
	fprintf(stderr, "table insertion error\n");
	return;
}

void __cyg_profile_func_enter(void *this_fn, void *call_site) {
	update_stats_start(this_fn, call_site);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site) {
	update_stats_end(this_fn, call_site);
}
//#endif


enum ETIME {START = 0, END = 1};

typedef void(*cb_func_c)(char*);

struct timeval start, end;

struct DAP_PATTERN_LUT{
    char *pattern;
    cb_func_c cb;
};

void callback1(char *s);

struct DAP_PATTERN_LUT relut[] = {
    {"033A", &callback1},
};

void callback1(char *s){
    fprintf(stdout, "callback1 function called, pattern = %s\n", s);
    return;
}

void do_nothing1(void) {
	printf("do nothing 1\n");
	(*relut[0].cb)(relut[0].pattern);
}

void do_nothing2(void) {
	printf("do nothing 2\n");
	(*relut[0].cb)(relut[0].pattern);
}

void do_something(void) {
	(*relut[0].cb)(relut[0].pattern);
}

// Determine elapse time
double elapsed_time(enum ETIME sts, struct timeval *start, struct timeval *end)
{
	long long startusec = 0, endusec = 0;
	double elapsed = 0;

	do_nothing1();

	do_nothing2();

	do_something();

	(*relut[0].cb)(relut[0].pattern);

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

	while(1);
	exit(EXIT_SUCCESS);

}



