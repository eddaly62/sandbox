// listfile.c
// list files in s directory

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <stdint.h>
#include <assert.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

char *tokens[3] = {
    "fifo",
    "test",
    "c"
};

int find_file(const char *pathname, char *token[], int numtokens, uint16_t ftype) {

    assert(pathname != NULL);
    assert(token != NULL);
    assert(numtokens > 0);
    assert(ftype==S_IFBLK || ftype==S_IFCHR || ftype==S_IFIFO ||
           ftype==S_IFLNK || ftype==S_IFREG || ftype==S_IFSOCK);

    int i, n, ri;
    char *rs;
    DIR *dir;
    struct dirent *entity;
    struct stat sb;

    // open a directory stream
    dir = opendir(pathname);
    if (dir == NULL) {
        return -1;
    }

    // first read of directory stream
    entity = readdir(dir);
    if (entity == NULL) {
        closedir(dir);
        return -1;
    }

    while ( entity != NULL) {

        // scan file name for tokens
        for (i = 0, n = 0; i < numtokens; i++) {
            rs = strcasestr(entity->d_name, token[i]);
            if (rs != NULL) {
                // matched a token from the token list
                printf("found token: %s in %s\n", token[i], entity->d_name);
                // count the tokens matched in the file name
                n++;
            }
        }

        // if file name has all of the tokens in it
        if (n == numtokens) {

            // test the file type
            ri = stat(entity->d_name, &sb);
            if (ri == -1) {
                closedir(dir);
                return -1;
            }

            // validate file type
            if ((sb.st_mode & S_IFMT) == ftype) {
                // its the right file type
                printf("\tfound file: %s\n", entity->d_name);
                closedir(dir);
                return 0;
            }
        }

        // read the next file from the directory stream
        entity = readdir(dir);
    }

    // no match...with tokens and file type
    closedir(dir);
    return -1;
}

int list_files(const char *pathname) {

    DIR *dir;
    struct dirent *entity;

    dir = opendir(pathname);
    if (dir == NULL) {
        return -1;
    }

    entity = readdir(dir);
    if (entity == NULL) {
        return -1;
    }

    // note: only d_name and d_ino are guarnteed across platforms
    while ( entity != NULL) {
        printf("%s: type(%d), length(%d), ino(%lu)\n", entity->d_name, entity->d_type, entity->d_reclen, entity->d_ino);
        entity = readdir(dir);
    }

    closedir(dir);
    return 0;
}

int main(int argc, char* argv[]) {

    int r;

    r = list_files(".");
    r = find_file(".", tokens, ARRAY_SIZE(tokens), S_IFREG);
    return 0;
}
