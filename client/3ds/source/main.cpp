#include <3ds.h>

#include "./../../common/net.h"
#include "./../../common/mainProcess.h"


int main() {
    atexit(gfxExit);
    atexit(closeNet);
    atexit(amExit);

    amInit();
    mainProcess(KEY_START);
}


