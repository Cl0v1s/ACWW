#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <malloc.h>

#include "./../../common/utils.h"
#include "./../../common/files.h"
#include "./../../common/config.h"
#include "./../../common/net.h"
#include "./../../common/letter.h"
#include "./../../common/letterFactory.h"
#include "./../../common/postman.h"
#include "./../../common/bootstrap.h"

#include <3ds.h>

#define SAVE_LENGTH 0x3FFFF

int main() {
    atexit(gfxExit);
    atexit(closeNet);
    atexit(amExit);

    amInit();
    initConsole();
    initFiles();
    // We need to put that data on stack, it doesnt work on rom
    char* saveData = (char*)malloc(sizeof(char) * SAVE_LENGTH);
    Config config = loadConfig("./ac.txt");
    printConfig(config);
    initNet(config.server.c_str(), config.port);

    if(!readFile(config.save.c_str(), saveData, SAVE_LENGTH)) {
        consolef("Unable to load save file\n");
        dsExit(1);
    }

    std::string backup = config.save + ".bak";
    if(!writeFile(backup.c_str(), saveData, SAVE_LENGTH)) {
        consolef("Unable to write backup file\n");
        dsExit(1);
    }

    LetterMemory* region = &LETTER_MEMORY_EUR_USA;
    Letter* letters = (Letter*)malloc(region->POST_BOX_LENGTH * sizeof(Letter));
    int letterLength = gatherLetter(saveData, letters, region);
    consolef("Loaded %d letters\n", letterLength);
    // for(int i = 0; i < letterLength; i++) {
    //     print(letters[i]);
    // }
    int delivered = deliverLetters(saveData, letters, letterLength, region, config.lang.c_str());
    consolef("%d letters got a reply !\n", delivered);
    checksum(saveData);
    if(!writeFile(config.save.c_str(), saveData, SAVE_LENGTH)) {
        consolef("Unable to write save file\n");
        dsExit(1);
    }
    consolef("Save edited !\n");
    free(saveData);
    if(config.launcher) {
        consolef("Press start to launch the game\n");
        waitForKey(KEY_START);
        dsStartWW(config);
        dsExit(0);
    } 
    dsExit(0);
}


