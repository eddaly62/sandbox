// testesc.c
// small program to see if \e translates to the ESC (27) sequence 

#include <stdio.h>
#include <string.h>


char *s = "\e[1m";
char *t = "\033[1m";

void main (void) {

    fprintf(stdout, s, strlen(s));
    fprintf(stdout, t, strlen(t));

}
