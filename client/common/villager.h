#ifndef VILLAGER_H
#define VILLAGER_H

#include <stdint.h>
#include <string>
#include <cstring>
#include <time.h>

#include "utils.h"
#include "translator.h"

#define MEMORIES_COUNT 8

typedef struct {
    uint16_t TOWN_ID;
    uint16_t TOWN_NAME;
    uint8_t TOWN_NAME_LENGTH;
    uint16_t PLAYER_ID;
    uint16_t PLAYER_NAME;
    uint8_t PLAYER_NAME_LENGTH;
    uint16_t DATE_DAY;
    uint16_t DATE_MONTH;
    uint16_t DATE_YEAR;
    uint16_t FRIENDSHIP;
    uint16_t FLAGS; // unused for now
} VillagerMemoryStruct;

typedef struct {
    uint16_t VILLAGER_START;
    uint16_t VILLAGER_SIZE;
    
    uint16_t MEMORIES_START;
    uint8_t MEMORIES_SIZE;

    uint16_t LETTER;
    uint16_t FURNITURE;
    uint16_t PERSONALITY;
    uint16_t VILLAGER_ID;
    uint16_t SHIRT;
    uint16_t WALLPAPER;
    uint16_t CARPET;
    uint16_t UMBRELLA;
} VillagerStruct;

VillagerMemoryStruct MEMORY_EUR_USA = {
    .TOWN_ID = 0x02,
    .TOWN_NAME = 0x04,
    .TOWN_NAME_LENGTH = 8,

    .PLAYER_ID = 0x0C,
    .PLAYER_NAME = 0x0E,
    .PLAYER_NAME_LENGTH = 8,

    .FRIENDSHIP = 0x8A7E - 0x8A3A,
    .DATE_DAY = 0x8A7F - 0x8A3A,
    .DATE_MONTH = 0x8A80 - 0x8A3A,
    .DATE_YEAR = 0x8A81 - 0x8A3A
};

VillagerMemoryStruct MEMORY_JPN = {};

VillagerMemoryStruct MEMORY_KOR = {};

VillagerStruct VILLAGER_EUR_USA = {
    .VILLAGER_START = 0x8A3C,
    .VILLAGER_SIZE = 0x700,
	.LETTER = 0x568,
	.FURNITURE = 0x6AC, // 10.
	.PERSONALITY = 0x6CA, 
	.VILLAGER_ID = 0x6CB, 
	.SHIRT = 0x6EC, 
	.WALLPAPER = 0x6EE, 
	.CARPET = 0x6EF, 
	.UMBRELLA = 0x6F4,

    .MEMORIES_SIZE = 0x8AA2 - 0x8A3A,
    .MEMORIES_START = 0x8A3A - 0x8A3C,
};

VillagerStruct VILLAGER_JPN = {
    .VILLAGER_START = 0x744C,
    .VILLAGER_SIZE = 0x5C0,
    .LETTER = 0x4A0,
	.FURNITURE = 0x578, // 10.
	.PERSONALITY = 0x594, 
	.VILLAGER_ID = 0x595, 
	.SHIRT = 0x5AE, 
	.WALLPAPER = 0x5B0, 
	.CARPET = 0x5B1, 
	.UMBRELLA = 0x544,

    .MEMORIES_SIZE = 0,
    .MEMORIES_START = 0,
};

VillagerStruct VILLAGER_KOR = {
    .VILLAGER_START = 0x9284,
    .VILLAGER_SIZE = 0x7EC,
    .LETTER = 0x634,
	.FURNITURE = 0x78C, // 10.
	.PERSONALITY = 0x7AE, 
	.VILLAGER_ID = 0x7AF, 
	.SHIRT = 0x7D2, 
	.WALLPAPER = 0x7D4, 
	.CARPET = 0x7D5, 
	.UMBRELLA = 0x7DA,

    .MEMORIES_SIZE = 0,
    .MEMORIES_START = 0,
};


class Memory {
    private:
        uint8_t* saveData;
        int startOffset;
        VillagerMemoryStruct* regionalData;
    public:
        Memory();

        void init(uint8_t* save, int offset, VillagerMemoryStruct* region) {
            saveData = save;
            startOffset = offset;
            regionalData = region;
        }

        uint16_t GetTownId() {
            return (saveData[startOffset + regionalData->TOWN_ID] << 8) | (saveData[startOffset + regionalData->TOWN_ID + 1] & 0xFF);
        }

        void SetTownId(uint16_t value) {
            saveData[startOffset + regionalData->TOWN_ID] = value >> 8;
            saveData[startOffset + regionalData->TOWN_ID + 1] = value & 0xFF;
        }

        uint16_t GetPlayerId() {
            return (saveData[startOffset + regionalData->PLAYER_ID] << 8) | (saveData[startOffset + regionalData->PLAYER_ID + 1] & 0xFF);
        }

        void SetPlayerId(uint16_t value) {
            saveData[startOffset + regionalData->PLAYER_ID] = value >> 8;
            saveData[startOffset + regionalData->PLAYER_ID + 1] = value & 0xFF;
        }

        std::wstring GetTownName() {
            return decode(saveData + startOffset + regionalData->TOWN_NAME, regionalData->TOWN_NAME_LENGTH, regionalData == &MEMORY_JPN, regionalData == &MEMORY_KOR);
        }
        void SetTownName(const std::wstring& value) {
            uint8_t output[value.length()];
            encode(value, output, regionalData == &MEMORY_JPN, regionalData == &MEMORY_KOR);
            memccpy(saveData + startOffset + regionalData->TOWN_NAME, output, sizeof(uint8_t), regionalData->TOWN_NAME_LENGTH);
        }

        std::wstring GetPlayerName() {
            return decode(saveData + startOffset + regionalData->PLAYER_NAME, regionalData->PLAYER_NAME_LENGTH, regionalData == &MEMORY_JPN, regionalData == &MEMORY_KOR);
        }
        void SetPlayerName(const std::wstring& value) {
            uint8_t output[value.length()];
            encode(value, output, regionalData == &MEMORY_JPN, regionalData == &MEMORY_KOR);
            memccpy(saveData + startOffset + regionalData->PLAYER_NAME, output, sizeof(uint8_t), regionalData->PLAYER_NAME_LENGTH);
        }

        time_t GetDate() {
            uint8_t day = saveData[startOffset + regionalData->DATE_DAY];
            uint8_t month = saveData[startOffset + regionalData->DATE_MONTH];
            uint8_t year = 2000 + saveData[startOffset + regionalData->DATE_YEAR];

            struct tm  tm;
            time_t rawtime;
            time ( &rawtime );
            tm = *localtime ( &rawtime );
            tm.tm_year = year - 1900;
            tm.tm_mon = month - 1;
            tm.tm_mday = day;
            return mktime(&tm);
        }

        void SetDate(time_t time) {
            struct tm* tm = localtime(&time);
            saveData[startOffset + regionalData->DATE_DAY] = tm->tm_mday;
            saveData[startOffset + regionalData->DATE_MONTH] = tm->tm_mon + 1;
            saveData[startOffset + regionalData->DATE_YEAR] = tm->tm_year - 100; // Since tm_year is years since 1900
        }

        uint8_t GetFriendship() {
            return saveData[startOffset + regionalData->FRIENDSHIP];
        }

        void SetFriendship(uint8_t value) {
            saveData[startOffset + regionalData->FRIENDSHIP] = value;
        }
};

class Villager {
    private:
        uint8_t* saveData;
        int startOffset;
        VillagerStruct* regionalData;
        Memory memories[MEMORIES_COUNT];

        void loadMemories() {
            VillagerMemoryStruct* region;
            if(regionalData == &VILLAGER_EUR_USA) {
                region = &MEMORY_EUR_USA;
            } else {
                printf("We only support EUR_USA for now.\n");
                dsExit(1);
                return;
            }

            for(int i = 0; i < MEMORIES_COUNT; i++) {
                memories[i].init(saveData, startOffset + regionalData->MEMORIES_START + i * regionalData->MEMORIES_SIZE, region);
            }
        }
    public:
        Villager(uint8_t* save, int offset, VillagerStruct* region) {
            saveData = save;
            startOffset = offset;
            regionalData = region;
            
            this->loadMemories();
        }


};

#endif 