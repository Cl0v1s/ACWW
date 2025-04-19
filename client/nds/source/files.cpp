#include "../../common/files.h"

#include <nds.h>
#include <fat.h>
#include <cstdio>

void initFiles()
{
    int res = 0;
    res = fatInitDefault();
    if(!res) {
        consolef("Unable to init filesystem %d\n", res);
        dsExit(1);
    }
}

bool readFile(const char *filename, char* content, int length)
{
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
    return true;
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