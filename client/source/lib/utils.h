#ifndef UTILS_H
#define UTILS_H

#include <cstdarg>

#ifdef ARM9

#include <nds.h>

#endif

#include "letter.h"

void logInit() {
    #ifdef ARM9
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D); //sub bg 0 will be used to print text
	vramSetBankA(VRAM_A_MAIN_BG);
	consoleDemoInit();
	// set up our bitmap background
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
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

#endif