#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <cstdarg>


#ifdef ARM9

#include <nds.h>
#include "leaf.h"

#elif __3DS__

#include <3ds.h>

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

    #elif defined(__3DS__)
        gfxInitDefault();
        consoleInit(GFX_BOTTOM,NULL);
    #endif
}


void consolef(const char* tmpl, ...) {
    va_list args;
    va_start(args, tmpl);

    #if defined(ARM9) || defined(__3DS__)
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
    #elif __3DS__
        while (aptMainLoop())
        {
            gspWaitForVBlank();
            hidScanInput();

            u32 kDown = hidKeysDown();
            if (kDown & key)
                break; // break in order to return to hbmenu

        }
    #endif 
}

void dsExit(int code) {
    #if defined(ARM9) || defined(__3DS__)
        consolef("Press start to exit...\n");
        waitForKey(KEY_START);
    #endif
    exit(code);
}

#endif