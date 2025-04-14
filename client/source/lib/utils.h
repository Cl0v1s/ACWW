#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <cstdarg>
#include <stdio.h>

#ifdef ARM9

#include <nds.h>

#endif

void initConsole() {
    #ifdef ARM9
	consoleDemoInit();
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
            int keys = keysDown();
            if(keys & key) break;
        }
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