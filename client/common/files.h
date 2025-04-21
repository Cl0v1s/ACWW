#ifndef FILES_H
#define FILES_H

#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sys/stat.h>

#include "utils.h"

#include <stdio.h>

#define CHECKSUM_OFFSET 0x15FDC
#define SAVCOPY_OFFSET 0x15FE0
#define SAVCOPY_SIZE 0x15FE0

void initFiles();

static inline bool writeFile(const char *filename, uint8_t *content, int length)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        return false;
    }

    fwrite(content, sizeof(char), length, fp);

    fclose(fp);
    return true;
}

static inline bool readFile(const char *filename, uint8_t* content, int length)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        consolef("File %s does not exist.\n", filename);
        return false;
    }

    fread(content, length, 1, fp);
    fclose(fp);
    return true;
}

static inline void checksum(uint8_t *save)
{
    uint16_t checksVar = 0;
    for (int index = 0; index < SAVCOPY_SIZE; index += 2)
    {
        if (index == CHECKSUM_OFFSET)
            continue;
        uint16_t cell = (save[index] & 0xFF) |
                        (save[index + 1] << 8);
        checksVar = (checksVar + cell) % 0x10000;
    }

    uint16_t checksum = 0x10000 - checksVar;
    save[CHECKSUM_OFFSET + 1] = checksum >> 8;
    save[CHECKSUM_OFFSET] = checksum & 0xFF;
    memmove(save + SAVCOPY_OFFSET, save, SAVCOPY_SIZE);
}


#endif