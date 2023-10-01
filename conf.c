// conf.c
// read and process a config file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

// get checksum of file
// returns -1 if error otherwise number of bytes read (1)
int get_file_checksum(char *pathname, uint8_t *chksum) {

    assert(pathname != NULL);
    int fd;
    uint8_t r;
    uint8_t sin;

    fd = open(pathname, (O_CREAT |O_RDWR), (S_IWUSR | S_IRUSR));
    if (fd == -1){
        // file does not exist
        return -1;
    }

    r = read(fd, chksum, 1);

    close(fd);
    return r;
}

// get checksum of file
// returns -1 if error otherwise number of bytes saved (1)
int save_file_checksum(char *pathname, uint8_t *chksum) {

    assert(pathname != NULL);
    int fd;
    uint8_t r;
    uint8_t sin;

    fd = open(pathname, (O_CREAT |O_RDWR), (S_IWUSR | S_IRUSR));
    if (fd == -1){
        // file does not exist
        return -1;
    }

    r = write(fd, chksum, 1);

    close(fd);
    return r;
}

// get checksum of file
// returns checksum otherwise error
int calc_file_checksum(char *pathname) {

    assert(pathname != NULL);
    int fd;
    uint8_t r;
    uint8_t sin;

    fd = open(pathname, O_RDONLY, (S_IRUSR | S_IRGRP | S_IROTH));
    if (fd == -1){
        // file does not exist
        return -1;
    }

    r = 0;
    while(read(fd,&sin, 1) != 0){
        r += sin;
    }
    printf("file(%s) checksum(0x%02x)\n", pathname, r);

    close(fd);
    return r;
}

// get file stats on config file
int conf_data_file_stats(char *pathname) {

    assert(pathname != NULL);
    struct stat sb;
    int r;
    uint8_t c1, c2;

    if (stat(pathname, &sb) == -1) {
        perror("Error: could not get file stat ");
        return -1;
    }

    printf("File size:                %lld bytes\n", (long long) sb.st_size);
    printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

    r = calc_file_checksum("confchecksumtest.conf");
    r = calc_file_checksum(pathname);
    if (r == -1) {
        printf("Error getting %s file checksum, file may not exist\n", pathname);
    }
    // save checksum to a hiden file
    c1 = (uint8_t)r;
    r = save_file_checksum(".confchksum", &c1);

    r = get_file_checksum(".confchksum", &c2);
    if (r == -1){
        printf("Error reading in check sum value\n");
    }
    else{
        printf("read in 0x%02x from confchksum\n", c2);
    }

    return r;
}


// parses a line of the configure file
int conf_parse_line(char *si, char *so[]) {

    assert(si != NULL);
    int i;
    char *token;
    char *sic, *sicc;

    // strtok is destructive on input string so make a duplicate
    i = 0;
    sic = strdup(si);
    sicc = sic;

    if ((si[0] == '#') || (si[0] == '\n')) {
        // line is a comment or blank
        return 0;
    }

    // parse string
    while (token != NULL) {
        if (i == 0) {
            token = strtok_r(sic, " ,=\n", &sicc);
        }
        else {
            token = strtok_r(NULL, " ,=\n", &sicc);
        }

        if (token != NULL) {
            strcpy(so[i], token);
            printf("length of token = %ld\n", strlen(token));
            printf("so[%d](%s), token(%s)\n", i, so[i], token);
            i++;
        }
    }

    printf("number of items found = %d\n", i);
    return i;
}

// config setting
int conf_set(char *so[]){

    // sample seting logic
    if(memcmp("fc", so[0], strlen("fc")) == 0){
        printf("setting %s\n",so[0]);
        return 0;
    }
    else if(memcmp("fb", so[0], strlen("fb")) == 0){
        printf("setting %s\n",so[0]);
        return 0;
    }
    else if(memcmp("vres", so[0], strlen("vres")) == 0){
        printf("setting %s\n",so[0]);
        return 0;
    }
    else if(memcmp("hres", so[0], strlen("hres")) == 0){
        printf("setting %s\n",so[0]);
        return 0;
    }
    else {
        printf("Nothing to set\n");
        return -1;
    }
}

#define MAX_LINE 128
int conf_read_file(char *pathname, char *so[]) {

    assert(pathname != NULL);
    int r;
    int len;
    FILE *fd;
    char si[100];

    // get status on config data file
    r = conf_data_file_stats(pathname);
    if (r == -1) {
        printf("Error getting file status");
    }

    // Open the file for reading
    fd = fopen (pathname, "r");
	if (fd == NULL) {
		fprintf(stderr, "Error: cannot open configure file(%s) in function(%s)\n", pathname, __FUNCTION__);
		return -1;
    }

    while (fgets(si, MAX_LINE, fd) != NULL) {

        //if (strlen(si) > 1) {
            // not a blank line
            // (a blank line contains only a \n)
            printf("readin %s\n", si);

            len = conf_parse_line(si, so);
            printf("len = %d\n", len);

            if (len > 1) {
                conf_set(so);
            }
        //}

    }

    fclose(fd);
}

int main(int argc, char *argv[])
{

    char *so[10];
    //int len;
    int n;
    int r;

    for (n=0; n<10; n++){
        so[n] = (char *)malloc(sizeof(char)*100);
    }

    r = conf_read_file("confdata.conf", so);
    if (r == -1){
        printf("could not open confdata.conf file\n");
    }

    for (n=0; n<10; n++){
        if (so[n] != NULL) {
            free(so[n]);
        }
    }
}
