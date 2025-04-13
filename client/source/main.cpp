
#define DEBUG false

#include "lib/sav.h"
#include "lib/letter.h"
#include "lib/postman.h"
#include "lib/utils.h"
#include "lib/net.h"

#include <string>
// #include <stdio.h>
// #include <nds.h>


int main() {
    initSave();
    initConsole();
    char* saveData;

    if(!readSave("misc/three-letters.sav", &saveData)) {
        consolef("Unable to load save file\n");
    }
    consolef("Loading letters\n");
    LetterMemory* region = &LETTER_MEMORY_EUR_USA;
    Letter* letters = (Letter*)malloc(region->POST_BOX_LENGTH * sizeof(Letter));
    int letterLength = gatherLetter(saveData, letters, region);
    consolef("Loaded %d letters\n", letterLength);
    for(int i = 0; i < letterLength; i++) {
        print(letters[i]);
    }
    deliverLetters(saveData, letters, 1, region);
    checksum(saveData);
    if(!writeSave("misc/save.sav", saveData)) {
        consolef("Unable to write save file\n");
    }
    consolef("Save edited, starting the game...\n");
    dsExit(0);
}