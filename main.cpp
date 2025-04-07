#include "lib/sav.h"
#include "lib/letter.h"

#include <string>

int main() {
    char* saveData;

    if(!readSave("misc/save.sav", &saveData)) {
        printf("Unable to load save file");
    }
    printf("Loading letter\n");

    Letter letter(saveData, 0x14918, &LETTER_EUR_USA);
    printf("Sender: %s\n", letter.GetSenderPlayerName().c_str());
    printf("Body: %s\n", letter.GetBodyPart().c_str());
}