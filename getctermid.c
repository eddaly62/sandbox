//#include <apue.h>
//#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <errno.h>
#include <ctype.h>

int main(void)
{
    char cterm[200];
    printf("Terminal ID path: %s\n", ctermid(cterm));
}