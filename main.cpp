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
    printf("Sender PlayerId: %04x\n", letter.GetSenderPlayerId());
    printf("Sender TownID: %04x\n", letter.GetSenderTownId());
    printf("Sender Player: %s\n", letter.GetSenderPlayerName().c_str());
    printf("Sender Town: %s\n", letter.GetSenderTownName().c_str());
    printf("Receiver Player: %s\n", letter.GetReceiverPlayerName().c_str());
    printf("Receiver PlayerId: %04x\n", letter.GetReceiverPlayerId());
    printf("Receiver TownId: %04x\n", letter.GetReceiverTownId());
    printf("Receiver Town: %s\n", letter.GetReceiverTownName().c_str());
    printf("Intro: %s\n", letter.GetIntroPart().c_str());
    printf("Body: %s\n", letter.GetBodyPart().c_str());
    printf("End: %s\n", letter.GetEndPart().c_str());
    printf("Flags: %04x\n", letter.GetFlags());
    printf("IntroIndex: %04x\n", letter.GetIntroIndex());
}