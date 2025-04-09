#ifndef POSTMAN_H
#define POSTMAN_H

#include "letter.h"
#include "letterFactory.h"

#define POST_BOX 0x1463C // May only be true for EUR_USA regions 
#define POST_BOX_LENGTH 10

#define MAIL_BOX 0x1200D // May only be true for EUR_USA regions
#define MAIL_BOX_LENGTH 10 

LetterFactory factory;

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

int gatherLetter(char* save, Letter* letters, LetterStruct* region) {
    return loadLetters(save, POST_BOX, POST_BOX_LENGTH, letters, region);
}

int getMailboxSlot(char* save, LetterStruct* region) {
    return loadLetters(save, MAIL_BOX, MAIL_BOX_LENGTH, NULL, region, true);
}

// Please be sure to have room in your mailbox 
void deliverLetters(char* save, Letter* letters, int length, LetterStruct* region) {
    bool done = false;
    int i = 0;
    while(done == false && i < length) {
        int slot = getMailboxSlot(save, region);
        if(slot >= MAIL_BOX_LENGTH) {
            printf("Not enough room in mailbox to write all the answers...\n");
            done = true;
        } else { 
            int offset = MAIL_BOX + slot * region->LETTER_SIZE;
            factory.Answer(letters[i], save, offset, region);
        }
        i += 1;
    }
}

#endif