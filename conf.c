// conf.c
// read and process a config file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct cf {
    int fd;
    char so[10][100];
} CONF;


int conf_parse_line(char *si, char *so[]) {

    assert(si != NULL);
    int i;
    char *token;
    char *sic, *sicc;

    if (si[0] == '#') {
        // line is a comment
        return 0;
    }

    // strtok is destructive on input string so make a duplicate
    i = 0;
    sic = strdup(si);
    sicc = sic;

    // parse string
    while (token != NULL) {
        if (i == 0) {
            token = strtok_r(sic, ",= ", &sicc);
        }
        else {
            token = strtok_r(NULL, ",= ", &sicc);
        }

        if (token != NULL) {
            memcpy(so[i], token, strlen(token));
            printf("so[%d] = %s\n", i, so[i]);
            i++;
        }
    }

    printf("number of items found = %d\n", i);
    return i;
}

// config setting
int conf_set(char *sarg[]){

    int r;
    r = memcmp("fg", sarg[0], strlen(sarg[0]));
    if(r == 0){
        printf("setting %s\n",sarg[0]);
        return 0;
    }
    else {
        printf("Nothing to set\n");
        return -1;
    }
}

int main(int argc, char *argv[])
{

    char *so[10];
    char *s = "fg = 123, 234, 345";
    int len;
    int n;
    CONF cf;

    for (n=0; n<10; n++){
        so[n] = (char *)malloc(sizeof(char)*100);
    }

    len = conf_parse_line(s, so);
    printf("len = %d\n", len);

    if (len > 1) {
        conf_set(so);
    }

#if 0
    if (argc != 2) {
		printf("Usage: ./fbinfo /dev/fb0\n");
		exit(1);
	}
 
	if (strstr(argv[1], "fb") == NULL) {
		printf("Only frame buffers (fb) are supported with this example\n");
		exit(1);
	}		

    // Open the file for reading and writing
	fbfd = open(argv[1], O_RDWR);
	if (fbfd == -1) {
		perror("Error: cannot open framebuffer device");
		goto handle_allocate_error;
	}
#endif

    for (n=0; n<10; n++){
        if (so[n] != NULL) {
            free(so[n]);
        }
    }
}
