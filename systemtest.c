// systemtest.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main (void) {

    sleep (5);
    system("echo \"Test of system\"");
    sleep (5);


}