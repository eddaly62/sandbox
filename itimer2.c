// itimer2.c
#define _POSIX_C_SOURCE 199309
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

static int i = 0;

typedef struct it {
    pthread_attr_t attr;
    struct sigevent sig;
    timer_t timerid;
    struct itimerspec in, out;
} INT_TIMER;

INT_TIMER itmr = {
    .in.it_value.tv_sec = 1,
    .in.it_value.tv_nsec = 0,
    .in.it_interval.tv_sec = 1,
    .in.it_interval.tv_nsec = 0,
    .sig.sigev_notify = SIGEV_THREAD,
    .sig.sigev_value.sival_int = 21,
    .sig.sigev_notify_attributes = &itmr.attr,
};

// Thread function to be invoked when the periodic timer expires
void sighler (union sigval val)
{
    printf("Handler entered with value :%d for %d times\n", val.sival_int, ++i);
}

// initialize and start a timer
int interval_timer(INT_TIMER *itmr, void *func) {

    int ret;

    pthread_attr_init(&itmr->attr);
    itmr->sig.sigev_notify_function = func;

    ret = timer_create(CLOCK_REALTIME, &itmr->sig, &itmr->timerid);
    if (ret == 0) {

        // issue the periodic timer request here.
        ret = timer_settime(itmr->timerid, 0, &itmr->in, &itmr->out);
        if(ret != 0) {
            ret = timer_delete(itmr->timerid);
        }
    }
    return ret;
}

int main()
{
    int ret;

    ret = interval_timer(&itmr, sighler);

    if(ret == 0)
        sleep(11);
    else
        printf("interval_timer() failed with %d\n", errno);
}