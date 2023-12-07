// get_process_name.c
// this program gets the process name given the process ID

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>

// returns length of name otherwise -1 if there was an error
// pid - pid to examine
// name - a pointer to were to store the name
// len - maximum size of array where name will get stored
int get_process_name_by_pid(int pid, char *name, size_t len)
{
    FILE *f;
    size_t size;

    assert(name != NULL);
    assert(len > 0);

    if(name != NULL){
        sprintf(name, "/proc/%d/cmdline", pid);
        f = fopen(name, "r");
        if(f != NULL) {
            size = fread(name, sizeof(char), len, f);
            printf("name = (%s)\n", name);
            if(size > 0) {
                if('\n' == name[size-1])
                    name[size-1]='\0';
            }
            fclose(f);
            printf("size = %ld\n", size);
            return (int)size;
        }
        else {
            // could not open file
            return -1;
        }
    }
    else {
        // error
        return -1;
    }
}


int main(int argc, char* argv[]) {

    int r;
    char name[1024];

    if (argc != 2) {
        printf("Usage:\n\t%s <PID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    r = get_process_name_by_pid(atoi(argv[1]), name, sizeof(name));
    if (r == -1) {
        printf("Could not resolve name of pid = %s\n", argv[1]);
    }
    else {
        printf("PID(%s) is (%s), size of name(%d)\n", argv[1], name, r);
    }

    exit(EXIT_SUCCESS);
}