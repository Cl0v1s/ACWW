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