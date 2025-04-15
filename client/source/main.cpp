
#define DEBUG false

#include "lib/files.h"
#include "lib/letter.h"
#include "lib/postman.h"
#include "lib/utils.h"
#include "lib/net.h"
#include "lib/config.h"

#include <string>


int main() {
    initFiles();
    initConsole();
    // char* saveData;
    // Config config = loadConfig();
    // printConfig(config);
    // initNet(config.server.c_str(), config.port);

    // if(!readFile(config.path.c_str(), &saveData)) {
    //     consolef("Unable to load save file\n");
    //     dsExit(1);
    // }

    // std::string backup = config.path + ".bak";
    // if(!writeFile(backup.c_str(), saveData)) {
    //     consolef("Unable to write backup file\n");
    //     dsExit(1);
    // }

    // LetterMemory* region = &LETTER_MEMORY_EUR_USA;
    // Letter* letters = (Letter*)malloc(region->POST_BOX_LENGTH * sizeof(Letter));
    // int letterLength = gatherLetter(saveData, letters, region);
    // consolef("Loaded %d letters\n", letterLength);
    // // for(int i = 0; i < letterLength; i++) {
    // //     print(letters[i]);
    // // }
    // int delivered = deliverLetters(saveData, letters, letterLength, region, config.lang.c_str());
    // consolef("%d letters got a reply !\n");
    // checksum(saveData);
    // if(!writeFile(config.path.c_str(), saveData)) {
    //     consolef("Unable to write save file\n");
    //     dsExit(1);
    // }
    consolef("Save edited, starting the game...\n");
    dsStartWW();
}