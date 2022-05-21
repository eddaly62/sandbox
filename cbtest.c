// cbtest.c
// Test of call back functionality

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

typedef void(*cb_func_c)(char*);
typedef void(*cb_func_i)(int);

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

void main(void)
{
    (*relut[0].cb)(relut[0].pattern);
    return;
}
