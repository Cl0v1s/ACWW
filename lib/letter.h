#ifndef LETTER_H
#define LETTER_H

#include <stdint.h>
#include <string>
#include <cstring>

#include "translator.h"

typedef struct {
    uint16_t LETTER_SIZE;
    uint8_t TOWNNAME_LENGTH;
    uint8_t PLAYERNAME_LENGTH;

    uint16_t TOWNID_RECEIVER;
    uint16_t TOWNNAME_RECEIVER;
    uint16_t PLAYERID_RECEIVER;
    uint16_t PLAYERNAME_RECEIVER;

    uint16_t TOWNID_SENDER;
    uint16_t TOWNNAME_SENDER;
    uint16_t PLAYERID_SENDER;
    uint16_t PLAYERNAME_SENDER;

    uint16_t INTRO_PART;
    uint8_t INTRO_LENGTH;
    uint16_t BODY_PART;
    uint8_t BODY_LENGTH;
    uint16_t END_PART;
    uint8_t END_LENGTH;

    uint8_t INTRO_INDEX;
    uint8_t PAPER_ID;
    uint8_t FLAGS;
    uint16_t ATTACHMENT_ITEM;
} LetterStruct;

LetterStruct LETTER_EUR_USA = {
    .LETTER_SIZE = 0xF4,
    .TOWNNAME_LENGTH = 8,
    .PLAYERNAME_LENGTH = 8,

    .TOWNID_RECEIVER = 0x4,
    .TOWNNAME_RECEIVER = 0x6,
    .PLAYERID_RECEIVER = 0xE,
    .PLAYERNAME_RECEIVER = 0x10,

    .TOWNID_SENDER = 0x1C,
    .TOWNNAME_SENDER = 0x1E,
    .PLAYERID_SENDER = 0x26,
    .PLAYERNAME_SENDER = 0x28,

    .INTRO_PART = 0x34,
    .INTRO_LENGTH = 24,
    .BODY_PART = 0x4C,
    .BODY_LENGTH = 129,
    .END_PART = 0xCC,
    .END_LENGTH = 32,

    .INTRO_INDEX = 0xEC,
    .PAPER_ID = 0xED,
    .FLAGS = 0xEE,
    .ATTACHMENT_ITEM = 0xF0
};

LetterStruct LETTER_JPN = {
    .LETTER_SIZE = 0x8C,
    .TOWNNAME_LENGTH = 6,
    .PLAYERNAME_LENGTH = 6,

    .TOWNID_RECEIVER = 0x4,
    .TOWNNAME_RECEIVER = 0x6,
    .PLAYERID_RECEIVER = 0xC,
    .PLAYERNAME_RECEIVER = 0xE,

    .TOWNID_SENDER = 0x18,
    .TOWNNAME_SENDER = 0x1A,
    .PLAYERID_SENDER = 0x20,
    .PLAYERNAME_SENDER = 0x22,

    .INTRO_PART = 0x2C,
    .INTRO_LENGTH = 10,
    .BODY_PART = 0x36,
    .BODY_LENGTH = 64,
    .END_PART = 0x76,
    .END_LENGTH = 16,

    .INTRO_INDEX = 0x86,
    .PAPER_ID = 0x87,
    .FLAGS = 0x88,
    .ATTACHMENT_ITEM = 0x8A
};

LetterStruct LETTER_KOR = {
    .LETTER_SIZE = 0x100,
    .TOWNNAME_LENGTH = 6,
    .PLAYERNAME_LENGTH = 6,

    .TOWNID_RECEIVER = 0x4,
    .TOWNNAME_RECEIVER = 0x6,
    .PLAYERID_RECEIVER = 0x12,
    .PLAYERNAME_RECEIVER = 0x14,

    .TOWNID_SENDER = 0x24,
    .TOWNNAME_SENDER = 0x26,
    .PLAYERID_SENDER = 0x32,
    .PLAYERNAME_SENDER = 0x34,

    .INTRO_PART = 0x44,
    .INTRO_LENGTH = 10,
    .BODY_PART = 0x58,
    .BODY_LENGTH = 64,
    .END_PART = 0xD8,
    .END_LENGTH = 16,

    .INTRO_INDEX = 0xF8,
    .PAPER_ID = 0xF9,
    .FLAGS = 0xFA,
    .ATTACHMENT_ITEM = 0xFC
};

class Letter {
    private:
        char* saveData;
        int startOffset;
        LetterStruct* regionalData;

    public: 
        Letter(char* save, int offset, LetterStruct* region) {
            saveData = save;
            startOffset = offset;
            regionalData = region;
        }

        uint16_t GetReceiverTownId() {
            return saveData[startOffset + regionalData->TOWNID_RECEIVER] << 8 + saveData[startOffset + regionalData->TOWNID_RECEIVER + 1];
        }

        std::string GetReceiverTownName() {
            return std::string(saveData + startOffset + regionalData->TOWNNAME_RECEIVER, regionalData->TOWNNAME_LENGTH);
        }

        uint16_t GetReceiverPlayerId() {
            return saveData[startOffset + regionalData->PLAYERID_RECEIVER] << 8 + saveData[startOffset + regionalData->PLAYERID_RECEIVER + 1];
        }

        std::string GetReceiverPlayerName() {
            return std::string(saveData + startOffset + regionalData->PLAYERNAME_RECEIVER, regionalData->PLAYERNAME_LENGTH);
        }

        uint16_t GetSenderTownId() {
            return saveData[startOffset + regionalData->TOWNID_SENDER] << 8 + saveData[startOffset + regionalData->TOWNID_SENDER + 1];
        }
        
        std::string GetSenderTownName() {
            return std::string(saveData + startOffset + regionalData->TOWNNAME_SENDER, regionalData->TOWNNAME_LENGTH);
        }
        
        uint16_t GetSenderPlayerId() {
            return saveData[startOffset + regionalData->PLAYERID_SENDER] << 8 + saveData[startOffset + regionalData->PLAYERID_SENDER + 1];
        }
        
        std::string GetSenderPlayerName() {
            return decode(std::string(saveData + startOffset + regionalData->PLAYERNAME_SENDER, regionalData->PLAYERNAME_LENGTH), regionalData == &LETTER_JPN, regionalData == &LETTER_KOR);
        }
        
        std::string GetIntroPart() {
            return std::string(saveData + startOffset + regionalData->INTRO_PART, regionalData->INTRO_LENGTH);
        }

        void SetIntroPart(const std::string& intro) {
            std::string encodedIntro = encode(intro, regionalData == &LETTER_JPN, regionalData == &LETTER_KOR);
            const char* cintro = encodedIntro.c_str();
            strncpy(saveData + startOffset + regionalData->INTRO_PART, cintro, regionalData->INTRO_LENGTH);
        }

        std::string GetBodyPart() {
            return decode(std::string(saveData + startOffset + regionalData->BODY_PART, regionalData->BODY_LENGTH), regionalData == &LETTER_JPN, regionalData == &LETTER_KOR);
        }

        void SetBodyPart(const std::string& body) {
            std::string encodedBody = encode(body, regionalData == &LETTER_JPN, regionalData == &LETTER_KOR);
            const char* cbody = encodedBody.c_str();
            strncpy(saveData + startOffset + regionalData->BODY_PART, cbody, regionalData->BODY_LENGTH);
        }

        std::string GetEndPart() {
            return std::string(saveData + startOffset + regionalData->END_PART, regionalData->END_LENGTH);
        }

        void SetEndPart(const std::string& end) {
            std::string encodedEnd = encode(end, regionalData == &LETTER_JPN, regionalData == &LETTER_KOR);
            const char* cend = encodedEnd.c_str();
            strncpy(saveData + startOffset + regionalData->END_PART, cend, regionalData->END_LENGTH);
        }

        uint8_t GetIntroIndex() {
            return saveData[startOffset + regionalData->INTRO_INDEX];
        }

        uint8_t GetPaperId() {
            return saveData[startOffset + regionalData->PAPER_ID];
        }

        uint8_t GetFlags() {
            return saveData[startOffset + regionalData->FLAGS];
        }

        uint16_t GetAttachementId() {
            return saveData[startOffset + regionalData->ATTACHMENT_ITEM] << 8 + saveData[startOffset + regionalData->ATTACHMENT_ITEM + 1];
        }

        void SetAttachementId(uint16_t id) {
            saveData[startOffset + regionalData->ATTACHMENT_ITEM] = id << 8;
            saveData[startOffset + regionalData->ATTACHMENT_ITEM+1] = ((uint8_t)(((uint32_t)(id)) & 0xFF));
        }


};

#endif
