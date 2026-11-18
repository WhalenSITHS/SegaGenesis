# Lesson: Plane-Based Parallax Scrolling on the Sega Genesis / Mega Drive (SGDK)

**Source Video:** [How to do Parallax Scrolling on the Sega Genesis & Mega Drive - Beginners Dev Tutorials](http://www.youtube.com/watch?v=UqC0Y0ZQmAo) by Pigsy's Retro Game Dev Tutorials

## Lesson Objectives

By the end of this lesson, you will be able to:

* Configure horizontal and vertical scrolling modes on the Mega Drive VDP \[00:02:56\].
* Understand the three structural sections of a `main.c` game program (declarations, one-time setup, and the 60fps `while` game loop) \[00:11:04\].
* Explain why background planes require $512 \times 256$ pixel dimensions to scroll seamlessly without empty gaps \[00:18:35\].
* Implement classic multi-layer parallax scrolling by moving `BG_B` and `BG_A` at different speeds \[00:23:26\].

## Part 1: Key Concepts

* **Plane-Based Parallax Scrolling:** The illusion of depth created by scrolling the foreground layer (`BG_A`) faster than the far background layer (`BG_B`) \[00:23:26\]. In this lesson, each entire plane scrolls as a single unit (`HSCROLL_PLANE` / `VSCROLL_PLANE`) \[00:03:27\], \[00:04:06\].
* **Tiles vs. Pixels:**
  * `VDP_drawImageEx()` places images using **$8 \times 8$ tile coordinates** (e.g., an offset of `10` moves an image 80 pixels) \[00:07:02\].
  * `VDP_setHorizontalScroll()` and `VDP_setVerticalScroll()` shift layers using **individual pixels**, enabling smooth frame-by-frame motion \[00:07:16\].
* **VDP Plane Resolution ($512 \times 256$):** Although the visible Genesis screen is $320 \times 224$ pixels, the Video Display Processor (VDP) stores background planes in a $512 \times 256$ pixel buffer \[00:18:35\]. Using a $512 \times 256$ image ensures off-screen tiles are always pre-loaded to wrap seamlessly without black gaps \[00:18:52\], \[00:20:16\].
* **Integer (`int`) Speed Limitation:** Standard `int` variables store whole numbers only \[00:08:53\]. Adding a decimal like `0.5` per frame rounds down to `0`, resulting in no movement—sub-pixel scrolling requires fixed-point data types (covered in later lessons) \[00:25:40\], \[00:26:10\].

## Part 2: Anatomy of `main.c`

To animate graphics, your C code must be organized into three distinct execution zones \[00:11:04\]:

1. **Top Section (Above `int main()`):** Header includes (`#include <genesis.h>`) and global variable declarations \[00:11:31\]. No CPU calculations or drawing functions can run here \[00:12:11\].
2. **Initialization Section (Inside `main()`, before `while(1)`):** Runs **once** when the ROM boots \[00:12:30\]. Ideal for loading palettes, uploading tiles to VRAM, drawing initial images, and setting the scrolling mode \[00:12:35\], \[00:15:07\].
3. **The Main Game Loop (`while(1)`):** Runs continuously **60 times per second** (once per frame) \[00:13:45\]. All per-frame updates—such as incrementing scroll variables, calling `VDP_setHorizontalScroll()`, and finishing with `SYS_doVBlankProcess()`—belong here \[00:14:18\], \[00:15:17\].

*(Warning: Putting VRAM loading or `VDP_drawImageEx()` inside the `while(1)` loop will repeatedly flood DMA bandwidth every frame and corrupt your graphics \[00:13:56\].)*

## Part 3: Step-by-Step Implementation

### 1. Prepare Seamless $512 \times 256$ Assets

1. Place two $512 \times 256$ PNG images (one background, one foreground) into your project's `res` folder \[00:20:23\], \[00:20:53\].
2. Update the filenames in `res/resources.res` (you can keep the same `bg1` and `fg1` variable names) and save \[00:21:12\]:

   ```text
   IMAGE bg1 "sonic_bg.png" BEST ALL
   IMAGE fg1 "sonic_fg.png" BEST ALL
   ```

### 2. Declare Scroll Tracking Variables

At the top of `src/main.c`, declare two integer variables to track the pixel offset of each layer \[00:08:53\], \[00:23:33\]:

```c
#include <genesis.h>
#include "resources.h"

int hscroll_offset = 0;      // Tracks BG_B (background) position
int hscroll_offset_fore = 0; // Tracks BG_A (foreground) position
```

### 3. Configure Scrolling Mode & Draw Layers (One-Time Setup)

Inside `main()`, load your palettes and draw both planes (shifted up by `-4` tiles if needed to center the $256$-pixel height on the $224$-pixel screen), then set the VDP scrolling mode to plane-based scrolling \[00:03:27\], \[00:04:06\], \[00:22:06\]:

```c
int main()
{
    u16 ind = TILE_USER_INDEX;

    // Load palettes and draw BG_B and BG_A
    PAL_setPalette(PAL0, bg1.palette->data, DMA);
    VDP_drawImageEx(BG_B, &bg1, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, -4, FALSE, TRUE);
    ind += bg1.tileset->numTile;

    PAL_setPalette(PAL1, fg1.palette->data, DMA);
    VDP_drawImageEx(BG_A, &fg1, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, -4, FALSE, TRUE);
    ind += fg1.tileset->numTile;

    // Set both horizontal and vertical scrolling to full-plane mode
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
```

### 4. Update Positions Inside the `while(1)` Loop

Inside the `while(1)` loop, apply the offsets to `BG_B` and `BG_A` and decrement the foreground faster (`-2`) than the background (`-1`) to scroll left with depth \[00:23:45\], \[00:24:09\]:

```c
    while(1)
    {
        // Apply current pixel offsets to each plane
        VDP_setHorizontalScroll(BG_B, hscroll_offset);
        VDP_setHorizontalScroll(BG_A, hscroll_offset_fore);

        // Move background slowly (1 px/frame) and foreground faster (2 px/frame)
        hscroll_offset -= 1;
        hscroll_offset_fore -= 2;

        // Wait for vertical blanking to render the frame cleanly
        SYS_doVBlankProcess();
    }
    return (0);
}
```

## Part 4: Experimentation & Exercises

| Modification | Code Change | Visual Effect |
| :--- | :--- | :--- |
| **Reverse Direction** | Change `-= 1` to `+= 1` \[00:17:36\] | Positive values scroll the layer to the **right**; negative values scroll to the **left** \[00:17:42\]. |
| **Adjust Scroll Speed** | Change `-= 2` to `-= 5` \[00:18:02\] | Increases movement speed to 5 pixels per frame (300 pixels/sec at 60fps) \[00:18:08\]. |
| **Add Vertical Scroll** | Add `VDP_setVerticalScroll(BG_B, hscroll_offset);` inside `while(1)` \[00:19:14\] | Scrolls the background vertically and horizontally simultaneously (diagonal motion) \[00:19:26\]. |

## Review Questions

1. **Why did using a $320 \times 224$ background image leave blank gaps while scrolling?**
   * *Answer:* The Mega Drive VDP plane buffer is $512 \times 256$ pixels wide; anything smaller leaves empty tiles in the buffer that become visible as the plane wraps \[00:18:35\].
2. **What happens if you place `VDP_drawImageEx()` inside the `while(1)` loop?**
   * *Answer:* It reloads and redraws the tiles 60 times every second, wasting resources and overloading DMA bandwidth until the screen corrupts \[00:13:56\].
3. **Why won't `hscroll_offset -= 0.5;` make the background scroll at half-pixel speed?**
   * *Answer:* Because `int` variables only store whole integers, so decimal increments are truncated/rounded and the offset never changes \[00:25:46\].