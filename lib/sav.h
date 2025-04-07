#ifndef SAV_H
#define SAV_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

bool readSave(const char* filename, char** content) {
    struct stat info;
    if (stat(filename, &info) != 0) {
        return false;
    }   
    (*content) = (char*)malloc(info.st_size);
    if ((*content) == NULL) {
        return false;
    }   

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return false;
    }

    size_t blocks_read = fread(*content, info.st_size, 1, fp);
    if (blocks_read != 1) {
        return false;
    }
    fclose(fp);
    return true;
}


#endif 