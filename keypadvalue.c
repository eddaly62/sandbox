// keypadvalue.c

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>

void main(void) {

    bool run = true;
    int i;

    while(run) {
        i = fgetc(stdin);
        printf("value = %d\n", i);
        if (i == '`')
            return;
    }

}