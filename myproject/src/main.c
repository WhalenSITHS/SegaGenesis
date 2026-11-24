#include <genesis.h>
#include "resources.h"
int h_scroll_offset = 1;
int hscroll_offset_fore = 0;
int main(bool hardReset)
{
    // VRAM setup
    u16 ind = TILE_USER_INDEX;
    PAL_setPalette(PAL0, green_hill_bg.palette->data, DMA);
    VDP_drawImageEx(BG_B, &green_hill_bg, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    // Foreground
    // Advance VRAM index past bg1's tiles
    ind += green_hill_bg.tileset->numTile;
    PAL_setPalette(PAL1, green_hill_fg.palette->data, DMA);
    VDP_drawImageEx(BG_A, &green_hill_fg, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    // Advance VRAM index past fg1's tiles for any future graphics
    ind += fg1.tileset->numTile;
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    VDP_setHorizontalScroll(BG_B, 10);
    while (TRUE)
    {
        SYS_doVBlankProcess();
    }
}