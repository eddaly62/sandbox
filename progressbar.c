// progressbar.c

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#define MAX_LEN 80
#define FILL_BACKGROUND "\e[47m"        // white
#define NORM_BACKGROUND "\e[42m"        // green
#define WARN_BACKGROUND "\e[43m"        // yellow
#define OVER_BACKGROUND "\e[41m"        // red
#define PROGRESS_BACKGROUND "\e[44m"    // blue
#define TEXT_DARK_COLOR "\e[30m"        // black
#define TEXT_LIGHT_COLOR "\e[37m"      // white

#define SPACE ' '
#define RESTORE "\e[0m"
#define CLR_SCREEN "\e[2J"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"

void statusbar(FILE *fd, char *label, int val, int max, int len, int ylevel, int rlevel) {

    int i;
    int l;
    int upc;
    int noc;
    char s[MAX_LEN];
    char t[MAX_LEN];
    char c;

    sprintf(s, "%d %s", val, label);
    l = strlen(s);
    
    upc = max/len;  // units per char
    noc = val/upc;  // number of chars in bar
    for (i = 0; i < len; i++) {
        if (i < l) {
            c = s[i];
        }
        else {
            c = SPACE;
        }

        if ((val < ylevel) && (i <= noc)) {
            sprintf(t, "%s%s%c", NORM_BACKGROUND, TEXT_LIGHT_COLOR, c);
        }
        else if ((val >= ylevel) && (val < rlevel) && (i <= noc)) {
            sprintf(t, "%s%s%c", WARN_BACKGROUND, TEXT_DARK_COLOR, c);
        }
        else if ((val >= rlevel) && (i <= noc)) {
            sprintf(t, "%s%s%c", OVER_BACKGROUND, TEXT_LIGHT_COLOR, c);
        }
        else {
            sprintf(t, "%s%s%c", FILL_BACKGROUND, TEXT_DARK_COLOR, c);
        }

        fprintf(fd,"%s", t);
        fflush(fd);

    }
    fprintf(fd, RESTORE);

}

void progressbar(FILE *fd, char *label, int val, int max, int len) {

    int i;
    int l;
    int upc;
    int noc;
    char s[MAX_LEN];
    char t[MAX_LEN];
    char c;

    sprintf(s, "%d %s", val, label);
    l = strlen(s);
    
    upc = max/len;  // units per char
    noc = val/upc;  // number of chars in bar
    for (i = 0; i < len; i++) {
        if (i < l) {
            c = s[i];
        }
        else {
            c = SPACE;
        }

        if (i <= noc) {
            sprintf(t, "%s%s%c", PROGRESS_BACKGROUND, TEXT_LIGHT_COLOR, c);
        }
        else {
            sprintf(t, "%s%s%c", FILL_BACKGROUND, TEXT_DARK_COLOR, c);
        }

        fprintf(fd,"%s", t);
        fflush(fd);

    }
    fprintf(fd, RESTORE);
}

int main(int argc, char *argv[])
{
    int x;
    fprintf(stdout, HIDE_CURSOR);
    for (x = 0; x <= 100; x++) {
        fprintf(stdout, "\r");
        statusbar(stdout, "unread", x, 100, 20, 50, 75);
        usleep(500*1000);
    }
    printf("\n\n");

    for (x = 0; x <= 100; x++) {
        fprintf(stdout, "\r");
        progressbar(stdout, "unread", x, 100, 20);
        usleep(500*1000);
    }

    //fprintf(stdout, RESTORE);
    printf("\n\n");
    fprintf(stdout, SHOW_CURSOR);

    return 0;
}

