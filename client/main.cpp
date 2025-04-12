
#define DEBUG false

#include "lib/sav.h"
#include "lib/letter.h"
#include "lib/postman.h"
#include "lib/utils.h"
#include "lib/net.h"

#include <string>

int main() {
    NetComputerImpl net("127.0.0.1", 8080);
    std::string content("Salut Nonos, Ça roule ? La bise");
    std::string answer = net.call("french", "002d", "Clovis", "Saintes", 0xf1fd, 100, content);
    printf(answer.c_str());
    // char* saveData;

    // if(!readSave("misc/three-letters.sav", &saveData)) {
    //     printf("Unable to load save file\n");
    // }
    // printf("Loading letters\n");
    // LetterMemory* region = &LETTER_MEMORY_EUR_USA;
    // Letter* letters = (Letter*)malloc(region->POST_BOX_LENGTH * sizeof(Letter));
    // int letterLength = gatherLetter(saveData, letters, region);
    // printf("Loaded %d letters\n", letterLength);
    // for(int i = 0; i < letterLength; i++) {
    //     print(letters[i]);
    // }
    // deliverLetters(saveData, letters, letterLength, region);
    // checksum(saveData);
    // if(!writeSave("misc/save.sav", saveData)) {
    //     printf("Unable to write save file\n");
    // }
}