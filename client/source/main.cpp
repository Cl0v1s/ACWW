
#define DEBUG false

#include "lib/sav.h"
#include "lib/letter.h"
#include "lib/postman.h"
#include "lib/utils.h"
#include "lib/net.h"

#include <string>
// #include <stdio.h>
// #include <nds.h>


int main() {
    logInit();
    char* saveData;

    if(!readSave("misc/three-letters.sav", &saveData)) {
        consolef("Unable to load save file\n");
    }
    consolef("Loading letters\n");
    LetterMemory* region = &LETTER_MEMORY_EUR_USA;
    Letter* letters = (Letter*)malloc(region->POST_BOX_LENGTH * sizeof(Letter));
    int letterLength = gatherLetter(saveData, letters, region);
    consolef("Loaded %d letters\n", letterLength);
    for(int i = 0; i < letterLength; i++) {
        print(letters[i]);
    }
    deliverLetters(saveData, letters, 1, region);
    checksum(saveData);
    if(!writeSave("misc/save.sav", saveData)) {
        consolef("Unable to write save file\n");
    }

    //     // the mode for 2 text layers and two extended background layers
	// videoSetMode(MODE_5_2D);

	// // set the sub background up for text display (we could just print to one
	// // of the main display text backgrounds just as easily
	// videoSetModeSub(MODE_0_2D); //sub bg 0 will be used to print text

	// vramSetBankA(VRAM_A_MAIN_BG);

	// consoleDemoInit();

	// iprintf("\n\n\tHello DS devers\n");
	// iprintf("\twww.drunkencoders.com\n");
	// iprintf("\t16 bit bitmap demo");

	// // set up our bitmap background
	// bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);

    // while(pmMainLoop()) {
	// 	swiWaitForVBlank();
	// 	scanKeys();
	// 	if (keysDown()&KEY_START) break;
	// }
}