#include "../../common/bootstrap.h"

#include <3ds.h>

void dsStartWW(const Config &config) {
    u64 romId = strtoull(config.rom.c_str(), nullptr, 16);
    if(romId == 0) {
        consolef("Rom entry in config is not set to valid title id.\n");
        dsExit(1);
    }
    int result;
    u32 length;
    result = AM_GetTitleCount(MEDIATYPE_SD, &length);
    if(result != 0) {
        consolef("Unable to count installed titles.\n");
        dsExit(1);
    }
    u64 titles[length];
    u32 read;
    result = AM_GetTitleList(&read, MEDIATYPE_SD, length, titles);
    if(result != 0) {
        consolef("Unable to list installed titles.\n");
        dsExit(1);
    }
    // bool done = false;
    // u32 i = 0;
    // while(i < length && done == false) {
    //     if(titles[i] == romId) {
    //         done = true;
    //     }
    //     i += 1;
    // }
    // if(done == false) {
    //     consolef("Unable to find title 0x%016llX.\n", romId);
    //     dsExit(1);
    // }
    consolef("Starting title 0x%016llX\n", romId);
    aptSetChainloader(romId, 0);
}