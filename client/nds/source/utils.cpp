#include "../../common/utils.h"

#include <nds.h>
#include "leaf.h"


void initConsole() {
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    consoleDemoInit();

    int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
    dmaCopy(leafBitmap, bgGetGfxPtr(bg3), 256*256);
    dmaCopy(leafPal, BG_PALETTE, 256*2);
}

void waitForKey(int key) {
    while(pmMainLoop()) {
        swiWaitForVBlank();
        scanKeys();
        int keys = keysDown();
        if(keys & key) break;
    }
    consolef("OK\n");
}