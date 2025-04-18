#ifndef POSTMAN_H
#define POSTMAN_H

#include <stdint.h>

#include "letter.h"
#include "letterFactory.h"

typedef struct {
    int POST_BOX;
    unsigned char POST_BOX_LENGTH;

    int MAIL_BOX;
    unsigned char MAIL_BOX_LENGTH;

} LetterMemory;

LetterMemory LETTER_MEMORY_EUR_USA = {
    .POST_BOX = 0x1463C,
    .POST_BOX_LENGTH = 10,

    .MAIL_BOX = 0x1200C,
    .MAIL_BOX_LENGTH = 10,
};

LetterFactory factory;

LetterStruct* selectRegion(LetterMemory* region) {
    if(region == &LETTER_MEMORY_EUR_USA) {
        return &LETTER_EUR_USA;
    }
    consolef("We only support EU/USA for now.\n");
    dsExit(1);
    return 0;
}

int loadLetters(char* save, int addr, int size,  Letter* letters, LetterStruct* region, bool dryRun = false) {
    int i = 0;
    bool done = false;
    while (done == false && i < size) {
        int offset = addr + i * region->LETTER_SIZE;
        Letter current(save, offset, region);
        if(current.Exists() == false) {
            done = true;
        } else {
            if(!dryRun) memcpy(&letters[i], &current, sizeof(Letter));
            i += 1;
        }
    }
    return i;
}

int gatherLetter(char* save, Letter* letters, LetterMemory* region) {

    return loadLetters(save, region->POST_BOX, region->POST_BOX_LENGTH, letters, selectRegion(region));
}

int getMailboxSlot(char* save, LetterMemory* region) {
    return loadLetters(save, region->MAIL_BOX, region->MAIL_BOX_LENGTH, NULL, selectRegion(region), true);
}

// Please be sure to have room in your mailbox 
int deliverLetters(char* save, Letter* letters, int length, LetterMemory* region, const char* lang) {
    bool done = false;
    int i = 0;
    LetterStruct* letterRegion = selectRegion(region);
    int delivered = 0;
    while(done == false && i < length) {
        int slot = getMailboxSlot(save, region);
        if(slot >= region->MAIL_BOX_LENGTH) {
            consolef("Not enough room in mailbox.\n");
            done = true;
        } else { 
            int offset = region->MAIL_BOX + slot * letterRegion->LETTER_SIZE;
            Letter ans = factory.Answer(letters[i], save, offset, letterRegion, lang);
            if(ans.Exists()) {
                delivered += 1;
            }
            // print(ans);
        }
        i += 1;
    }
    return delivered;
}

#endif