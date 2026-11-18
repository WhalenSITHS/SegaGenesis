#include <genesis.h>
#include "resources.h"

int main(bool hardReset)
{
    // VRAM setup
    u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, bg1.palette->data, DMA);
    VDP_drawImageEx(BG_B, &bg1, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    while (TRUE)
    {
        SYS_doVBlankProcess();
    }
    return 0;
}