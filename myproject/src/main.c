#include <genesis.h>
#include "resources.h"

int main(bool hardReset)
{
    // VRAM setup
    u16 ind = TILE_USER_INDEX;
    PAL_setPalette();
    while (TRUE)
    {
        SYS_doVBlankProcess();
    }
    return 0;
}