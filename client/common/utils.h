#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string>

#if defined(ARM9) 

#include <nds.h>

#elif defined(__3DS__)

#include <3ds.h>
#include <cstdarg>

#else 

#include <cstdarg>

#endif

void initConsole();

void waitForKey(int key);

/**
 * This incorrectly converts utf16 to ASCII, use only for debug printing
 */
static inline std::string convertToASCII(std::wstring input) {
    char output[input.length() + 1];
    for(unsigned int i = 0; i < input.length(); i++) {
        output[i] = (char)(input[i] & 0xff);
    }
    output[input.length()] = '\0';
    return std::string(output);
}

static inline void consolef(const char* tmpl, ...) {
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

static inline void dsExit(int code) {
    #if defined(ARM9) || defined(__3DS__)
        consolef("Press start to exit...\n");
        waitForKey(KEY_START);
    #endif
    exit(code);
}

#endif