#include <genesis.h>
#include "resources.h"

int main(bool hardReset)
{
    // VRAM setup
    u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, bg1.palette->data, DMA);
    VDP_drawImageEx(BG_B, &bg1, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    // Foreground
    // Advance VRAM index past bg1's tiles
    ind += bg1.tileset->numTile;
    PAL_setPalette(PAL1, fg1.palette->data, DMA);
    VDP_drawImageEx(BG_A, &fg1, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    // Advance VRAM index past fg1's tiles for any future graphics
    ind += fg1.tileset->numTile;
    while (TRUE)
    {
        SYS_doVBlankProcess();
    }
    return 0;
}