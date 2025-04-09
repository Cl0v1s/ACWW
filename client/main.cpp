#include "lib/sav.h"
#include "lib/letter.h"
#include "lib/postman.h"
#include "lib/utils.h"

#include <string>

int main() {
    char* saveData;

    if(!readSave("misc/three-letters.sav", &saveData)) {
        printf("Unable to load save file\n");
    }
    printf("Loading letters\n");
    Letter* letters = (Letter*)malloc(POST_BOX_LENGTH * sizeof(Letter));
    int letterLength = gatherLetter(saveData, letters, &LETTER_EUR_USA);
    printf("Loaded %d letters\n", letterLength);
    for(int i = 0; i < letterLength; i++) {
        print(letters[i]);
    }
    deliverLetters(saveData, letters, letterLength, &LETTER_EUR_USA);
    checksum(saveData);
    if(!writeSave("misc/save.sav", saveData)) {
        printf("Unable to write save file\n");
    }
}