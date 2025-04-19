#include "../../common/utils.h"

#include <3ds.h>
#include <cstring>
#include "leaf_bgr.h"


void initConsole() {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM,NULL);
    gfxSetDoubleBuffering(GFX_TOP, false);
    u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	memcpy(fb, leaf_bgr, leaf_bgr_size);
}

void waitForKey(int key) {
    while (aptMainLoop())
    {
        gspWaitForVBlank();
        hidScanInput();

        u32 kDown = hidKeysDown();
        if (kDown & key)
            break; // break in order to return to hbmenu

    }
}