// itimer.c
#define _POSIX_C_SOURCE 199309
//#define __USE_POSIX199309
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

//#define CLOCKID CLOCK_REALTIME
//#define SIG SIGRTMIN


static int i = 0;

//Thread function to be invoked when the periodic timer expires
void sighler (union sigval val)
{
    printf("Handler entered with value :%d for %d times\n", val.sival_int, ++i);
}

int main()
{
    int Ret;
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    struct sched_param parm;
    parm.sched_priority = 255;
    pthread_attr_setschedparam(&attr, &parm);
    struct sigevent sig;

    sig.sigev_notify = SIGEV_THREAD;
    sig.sigev_notify_function = sighler;
    sig.sigev_value.sival_int =20;
    sig.sigev_notify_attributes = &attr;

    //create a new timer.
    timer_t timerid;

    Ret = timer_create(CLOCK_REALTIME, &sig, &timerid);
    if (Ret == 0)
    {
        struct itimerspec in, out;
        in.it_value.tv_sec = 1;
        in.it_value.tv_nsec = 0;
        in.it_interval.tv_sec = 0;
        in.it_interval.tv_nsec = 100000000;
        //issue the periodic timer request here.
        Ret = timer_settime(timerid, 0, &in, &out);
        if(Ret == 0)
            sleep(2);
        else
            printf("timer_settime() failed with %d\n", errno);
        //delete the timer.
        timer_delete(timerid);
    }
    else
    {
    printf("timer_create() failed with %d\n", errno);
    return Ret;
    }
}