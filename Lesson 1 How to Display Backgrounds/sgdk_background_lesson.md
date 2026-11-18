# Lesson: Displaying Backgrounds on the Sega Genesis / Mega Drive (SGDK)

**Source Video:** [How to Display Backgrounds on the Sega Genesis & Mega Drive - Beginners Game Dev Tutorial (SGDK)](http://www.youtube.com/watch?v=8l5_FTgHkuM) by Pigsy's Retro Game Dev Tutorials

## Lesson Objectives
By the end of this lesson, you will be able to:
* Configure and import image assets into a Sega Genesis ROM using a `.res` file [00:00:56].
* Allocate VRAM tile space and assign color palettes using SGDK functions [00:04:24].
* Render both background (`BG_B`) and foreground (`BG_A`) layers on screen simultaneously [00:07:26].
* Manipulate tile priority, flipping, and tile-based screen coordinates [00:11:28].

---

## Part 1: Key Concepts
* **Dual Background Planes:** The Sega Mega Drive / Genesis hardware natively supports two scrollable background layers: `BG_B` (the far background) and `BG_A` (the foreground layer) [00:07:26].
* **Color Palettes (`PAL0`–`PAL3`):** The system displays up to 64 colors on screen, divided into four 16-color palettes (`PAL0`, `PAL1`, `PAL2`, and `PAL3`). Each image layer requires its palette data to be loaded into one of these slots [00:05:57].
* **VRAM Indexing (`TILE_USER_INDEX`):** Graphic images are broken down into $8 \times 8$ pixel tiles and stored in Video RAM (VRAM). You must track the starting VRAM index for each image so that loading a second graphic does not overwrite the tiles of the first [00:15:21].

---

## Part 2: Step-by-Step Implementation

### 1. Import Graphics via `resources.res`
1. Copy your project template folder and open it in Visual Studio Code [00:00:20].
2. Place your PNG background and foreground image files into the project's `res` folder [00:01:24].
3. Create a new file inside `res` named `resources.res` and declare your images using the exact syntax below (case-sensitive) [00:01:02]:
   ```text
   IMAGE bg1 "bg1.png" BEST ALL
   IMAGE fg1 "fg1.png" BEST ALL
   ```
   * **`IMAGE`**: Tells SGDK the resource type [00:01:56].
   * **`bg1` / `fg1`**: The variable name used to reference the asset in C code [00:02:20].
   * **`"bg1.png"`**: The exact filename in quotes [00:02:41].
   * **`BEST`**: Compression level [00:02:48].
   * **`ALL`**: Export setting [00:03:05].
4. Save `resources.res` (`Ctrl + S`) and compile the project (`Ctrl + Shift + P` -> Compile) so SGDK generates the header definitions for your C code [00:03:17], [00:15:04].

### 2. Include Resources & Track VRAM in `main.c`
Open `src/main.c` and include the generated resource header at the top of the file [00:04:01]:
```c
#include <genesis.h>
#include "resources.h"
```
Inside `main()`, initialize your VRAM tile index tracker [00:04:36]:
```c
u16 ind = TILE_USER_INDEX;
```

### 3. Load Palettes and Draw the Background (`BG_B`)
Assign the background image's palette to `PAL0` (recommended for the far background to prevent losing the first color index) and draw the image onto `BG_B` [00:06:09], [00:12:42]:
```c
// Load background palette into PAL0 via DMA
PAL_setPalette(PAL0, bg1.palette->data, DMA);

// Draw bg1 onto the background plane (BG_B) at tile coordinates (0, 0)
VDP_drawImageEx(BG_B, &bg1, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
```
* **`TILE_ATTR_FULL` parameters:** `(palette, priority, flipV, flipH, vram_index)` [00:08:24].
* **`VDP_drawImageEx` parameters:** `(plane, &image, tile_attributes, x_tiles, y_tiles, load_palette, use_dma)` [00:07:20].

### 4. Advance VRAM Index & Draw the Foreground (`BG_A`)
Before drawing the second layer, advance the VRAM index (`ind`) by the number of tiles used by `bg1` so the foreground tiles don't overwrite the background tiles [00:15:21]:
```c
// Advance VRAM index past bg1's tiles
ind += bg1.tileset->numTile;

// Load foreground palette into PAL1
PAL_setPalette(PAL1, fg1.palette->data, DMA);

// Draw fg1 onto the foreground plane (BG_A)
VDP_drawImageEx(BG_A, &fg1, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);

// Advance VRAM index past fg1's tiles for any future graphics
ind += fg1.tileset->numTile;
```

---

## Part 3: Experimentation & Exercises
Try modifying the parameters in `main.c` and recompiling to see how the Mega Drive VDP (Video Display Processor) responds:

| Parameter to Change | Code Modification | Expected Result |
| :--- | :--- | :--- |
| **Layer Priority** | Change `prio` to `TRUE` in `BG_B`'s `TILE_ATTR_FULL` [00:18:12] | `BG_B` renders *in front* of `BG_A`, hiding foreground objects behind the background [00:18:25]. (Useful when you want sprites to walk behind foreground lampposts or pillars [00:18:38]). |
| **Tile Flipping** | Change `flipV` or `flipH` to `TRUE` [00:13:07] | Every individual $8 \times 8$ tile in the image flips vertically or horizontally in place [00:13:19]. |
| **Screen Position** | Change `x` and `y` in `VDP_drawImageEx` from `0, 0` to `10, -5` [00:19:11], [00:19:40] | Shifts the layer right by 10 tiles (80 pixels) and up by 5 tiles (40 pixels) [00:19:21], [00:19:46]. |
| **Palette Swap** | Swap `PAL0` and `PAL1` inside `TILE_ATTR_FULL` [00:20:46] | Causes corrupted/inverted colors because each layer references the wrong color index table [00:20:57]. |

---

## Review Questions
1. **Why is `ind += bg1.tileset->numTile;` necessary before calling `VDP_drawImageEx` for `fg1`?**
   * *Answer:* Without advancing `ind`, the foreground tiles would load into the exact same VRAM address as the background tiles, overwriting them and causing visual corruption [00:15:35].
2. **If you set `x = 10` in `VDP_drawImageEx`, how many pixels does the image shift?**
   * *Answer:* 80 pixels, because coordinates in `VDP_drawImageEx` are measured in $8 \times 8$ pixel tiles rather than individual pixels [00:19:21].
3. **Where can you find the compiled ROM file to test on real hardware or a flashcart?**
   * *Answer:* Inside your project's `out` folder as `rom.bin` [00:21:57].