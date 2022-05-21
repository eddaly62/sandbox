#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define DAPU1RX "/tmp/dapu1rx"
#define DAPU1TX "/tmp/dapu1tx"
#define EXIT_STRING "q"
#define CONTINUE_STRING "c"
#define MAX_LEN 128

int fdu1rx;
int fdu1tx;
char s[MAX_LEN];
char c[MAX_LEN];
 
void fini (void) {
    unlink(DAPU1RX);
    unlink(DAPU1TX);
    close(fdu1rx);
    close(fdu1tx);
}

void main (void) {

    atexit(fini);
    

    fprintf(stdout,"1) Open two terminal windows\n");
    fprintf(stdout,"2) Type the following in one terminal:\n\tcat < /tmp/dapu1rx\n");
    fprintf(stdout,"3) Type the following in the other terminal:\n\tcat < /tmp/dapu1tx\n");

    unlink(DAPU1RX);
    unlink(DAPU1TX);

    if (mkfifo(DAPU1RX, 0666) == -1) {
        fprintf(stdout, "strerror(%s)\n", strerror(errno));
        fprintf(stdout, "Could not create DAPU1RX FIFO\n");
        return;
    }

    if (mkfifo(DAPU1TX, 0666) == -1) {
        fprintf(stdout, "strerror(%s)\n", strerror(errno));
        fprintf(stdout, "Could not create DAPU1TX FIFO\n");
        return;
    }

    fdu1rx = open(DAPU1RX, O_WRONLY);
    if (fdu1rx == -1) {
        fprintf(stdout, "strerror(%s)\n", strerror(errno));
        fprintf(stdout, "Could not open DAPU1RX FIFO\n");
        return;
    }

    fdu1tx = open(DAPU1TX, O_WRONLY);
    if (fdu1tx == -1) {
        fprintf(stdout, "strerror(%s)\n", strerror(errno));
        fprintf(stdout, "Could not open DAPU1TX FIFO\n");
        return;
    }

    write(fdu1rx, "rx:", strlen("rx:"));
    write(fdu1tx, "tx:", strlen("tx:"));


    while (1) {

        fprintf(stdout,"Enter message: ");
        if(fgets(s, MAX_LEN, stdin) > 0) {
            if (strcmp(EXIT_STRING, s) == 0) {
                return;
            }
        }
        write(fdu1rx, s, strlen(s));
        write(fdu1tx, s, strlen(s));

    }
}

