#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>


#ifdef ARM9

#include <nds.h>
#include "leaf.h"

#else 

#include <cstdarg>

#endif

void initConsole() {
    #ifdef ARM9
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	consoleDemoInit();

	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(leafBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(leafPal, BG_PALETTE, 256*2);
    #endif
}


void consolef(const char* tmpl, ...) {
    va_list args;
    va_start(args, tmpl);

    #ifdef ARM9 
        va_list args_copy;
        va_copy(args_copy, args);
        viprintf(tmpl, args_copy);
        va_end(args_copy);
    #else 
        vprintf(tmpl, args);
    #endif

    va_end(args);
}

void waitForKey(int key) {
    #ifdef ARM9
        while(pmMainLoop()) {
            swiWaitForVBlank();
            scanKeys();
            int keys = keysDown();
            if(keys & key) break;
        }
        consolef("OK\n");
    #endif
}

void dsExit(int code) {
    #ifdef ARM9
        consolef("Press start to exit...\n");
        waitForKey(KEY_START);
    #endif
    exit(code);
}

#endif