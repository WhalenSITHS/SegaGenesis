#include <genesis.h>

int main(bool hardReset)
{
    VDP_drawText("Hello Genesis!", 10, 12);
    while (TRUE)
    {
        SYS_doVBlankProcess();
    }
    return 0;
}