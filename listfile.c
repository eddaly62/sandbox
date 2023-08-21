// listfile.c
// list files in s directory

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <stdint.h>

char *tokens[3] = {
    "fifo",
    "test",
    "c"
};

int findfiles(const char *pathname, char *token[], int numtokens, uint16_t ftype) {

    int i, n, ri;
    char *rs;
    DIR *dir;
    struct dirent *entity;
    struct stat sb;

    dir = opendir(pathname);
    if (dir == NULL) {
        return -1;
    }

    entity = readdir(dir);
    if (entity == NULL) {
        closedir(dir);
        return -1;
    }

    while ( entity != NULL) {
        for (i = 0, n = 0; i < numtokens; i++) {
            rs = strcasestr(entity->d_name, token[i]);
            if (rs != NULL) {
                // matched a token from the token list
                printf("found token: %s in %s\n", token[i], entity->d_name);
                n++;
            }
        }
        if (n == numtokens) {
            // test file type
            ri = stat(entity->d_name, &sb);
            if (ri == -1) {
                closedir(dir);
                return -1;
            }
            if ((sb.st_mode & S_IFMT) == ftype) {
                // print matching file
                printf("found file: %s\n", entity->d_name);
            }
        }
        entity = readdir(dir);
    }

    closedir(dir);
    return 0;
}

int listfiles(const char *pathname) {

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

    while ( entity != NULL) {
        printf("%s: type(%d), length(%d), ino(%lu)\n", entity->d_name, entity->d_type, entity->d_reclen, entity->d_ino);
        entity = readdir(dir);
    }

    closedir(dir);
    return 0;
}

int main(int argc, char* argv[]) {

    //listfiles(".");
    findfiles(".", tokens, 3, S_IFREG);
    return 0;
}
