#ifndef SAV_H
#define SAV_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <sys/stat.h>

#define CHECKSUM_OFFSET 0x15FDC
#define SAVCOPY_OFFSET 0x15FE0
#define SAVCOPY_SIZE 0x15FE0

bool readSave(const char* filename, char** content) {
    struct stat info;
    if (stat(filename, &info) != 0) {
        return false;
    }   
    (*content) = (char*)malloc(info.st_size);
    if ((*content) == NULL) {
        return false;
    }   
    printf("%d\n", info.st_size);

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

void checksum(char *save) {
    uint16_t checksVar = 0;
	for(int index = 0; index < SAVCOPY_SIZE; index += 2) {
		if (index == CHECKSUM_OFFSET) continue;
        uint16_t cell = (save[index] & 0xFF) |
        (save[index+1] << 8);
		checksVar = (checksVar + cell) % 0x10000;
	}

    uint16_t checksum = 0x10000 - checksVar;
    save[CHECKSUM_OFFSET+1] = checksum >> 8;
    save[CHECKSUM_OFFSET] = checksum & 0xFF;
    memmove(save + SAVCOPY_OFFSET, save, SAVCOPY_SIZE);
}

bool writeSave(const char* filename, char* content) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return false;
    }

    fwrite(content, sizeof(char), 262144, fp);

    fclose(fp);
    return true;
}


#endif 