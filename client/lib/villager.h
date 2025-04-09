#ifndef VILLAGER_H
#define VILLAGER_H

#include <stdint.h>


typedef struct {
    uint16_t VILLAGER_START;
    uint16_t VILLAGER_SIZE;

    uint16_t LETTER;
    uint16_t FURNITURE;
    uint16_t PERSONALITY;
    uint16_t VILLAGER_ID;
    uint16_t SHIRT;
    uint16_t WALLPAPER;
    uint16_t CARPET;
    uint16_t UMBRELLA;
} VillagerStruct;

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