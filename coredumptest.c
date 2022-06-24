// coredumptest.c
// used to test core dump with gdb debugger

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

struct DATA {
    int i;
    int x;
};

int main (int argc, char *argv[]) {

    int r;
    struct DATA *d = NULL;

    r = d->x;

    return r;


}