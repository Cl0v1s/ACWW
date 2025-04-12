
#define DEBUG false

#include "lib/sav.h"
#include "lib/letter.h"
#include "lib/postman.h"
#include "lib/utils.h"
#include "lib/net.h"

#include <string>

int main() {
    char* saveData;

    if(!readSave("misc/three-letters.sav", &saveData)) {
        printf("Unable to load save file\n");
    }
    printf("Loading letters\n");
    LetterMemory* region = &LETTER_MEMORY_EUR_USA;
    Letter* letters = (Letter*)malloc(region->POST_BOX_LENGTH * sizeof(Letter));
    int letterLength = gatherLetter(saveData, letters, region);
    printf("Loaded %d letters\n", letterLength);
    for(int i = 0; i < letterLength; i++) {
        print(letters[i]);
    }
    deliverLetters(saveData, letters, 1, region);
    checksum(saveData);
    if(!writeSave("misc/save.sav", saveData)) {
        printf("Unable to write save file\n");
    }
}