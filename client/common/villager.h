#ifndef VILLAGER_H
#define VILLAGER_H

#include <stdint.h>

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
    uint16_t FLAGS;
} VillagerMemoryStruct;

typedef struct {
    uint16_t VILLAGER_START;
    uint16_t VILLAGER_SIZE;
    
    uint16_t MEMORIES_START;
    uint8_t MEMORIES_SIZE;
    uint8_t MEMORIES_LENGTH;
 

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

    .DATE_DAY = 0x8A7E - 0x8A3A,
    .DATE_DAY = 0x8A7F - 0x8A3A,
    .DATE_MONTH = 0x8A80 - 0x8A3A,
    .DATE_YEAR = 0x8A81 - 0x8A3A
};

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
	.UMBRELLA = 0x6F4 
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
	.UMBRELLA = 0x544 
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
	.UMBRELLA = 0x7DA 
};

class Villager {
    private:
        char* saveData;
        int startOffset;
        VillagerStruct* regionalData;

    public:
        Villager(char* save, int offset, VillagerStruct* region) {
            saveData = save;
            startOffset = offset;
            regionalData = region;
        }
};

#endif 