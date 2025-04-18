#ifndef FILES_H
#define FILES_H

#include <stdint.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sys/stat.h>

#include "utils.h"

#ifdef ARM9

#include <nds.h>
#include <fat.h>
#include <cstdio>

#else

#include <stdio.h>

#endif

#define CHECKSUM_OFFSET 0x15FDC
#define SAVCOPY_OFFSET 0x15FE0
#define SAVCOPY_SIZE 0x15FE0

void initFiles()
{
    int res = 0;
    #ifdef ARM9
        res = fatInitDefault();
    #endif
    if(!res) {
        consolef("Unable to init filesystem %d\n", res);
        dsExit(1);
    }
}

bool readFile(const char *filename, char* content, int length)
{
    #if defined(__3DS__)
        int result;
        FS_Archive sdmcArchive;
        std::string path = std::string("/");
        FS_Path dirPath = fsMakePath(PATH_ASCII, path.c_str());
        result = FSUSER_OpenArchive( &sdmcArchive, ARCHIVE_SDMC, dirPath);
        if(result != 0) {
            consolef("Unable to open SD card %d\n", result);
            return false;
        }
        Handle fp;
        path = path + std::string(filename);
        FS_Path filePath = fsMakePath(PATH_ASCII, path.c_str());
        result = FSUSER_OpenFile(&fp, sdmcArchive, filePath, FS_OPEN_READ, 0);
        if(result != 0) {
            consolef("Unable to open file %s\n", path.c_str());
            return false;
        }
        u32 blocks_read;
        result = FSFILE_Read(fp, &blocks_read, 0, content, length);
        if(result != 0) {
            consolef("Unable to read file %s\n", path.c_str());
            return false;
        }
        FSFILE_Close(fp);
        FSUSER_CloseArchive(sdmcArchive);
    #else 
        FILE* fp = fopen(filename, "rb");
        if (fp == NULL)
        {
            consolef("File %s does not exist.\n", filename);
            return false;
        }

        size_t blocks_read = fread(content, length, 1, fp);
        if (blocks_read != 1)
        {
            consolef("Unable to read file %s.\n", filename);
            return false;
        }
        fclose(fp);
    #endif
    
    return true;
}

void checksum(char *save)
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

bool writeFile(const char *filename, char *content)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        return false;
    }

    fwrite(content, sizeof(char), 262144, fp);

    fclose(fp);
    return true;
}

#endif