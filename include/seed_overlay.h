#ifndef POKEPLATINUM_SEED_OVERLAY_H
#define POKEPLATINUM_SEED_OVERLAY_H

#include "constants/heap.h"

#include "bg_window.h"

// Debug overlay: shows the current LCRNG state in the top-left corner of a screen.
// Comment this out to build without it. Every call site is guarded by it.
#define SHOW_RNG_SEED_OVERLAY

#define SEED_OVERLAY_WIDTH_TILES  9
#define SEED_OVERLAY_HEIGHT_TILES 2
#define SEED_OVERLAY_TILE_COUNT   (SEED_OVERLAY_WIDTH_TILES * SEED_OVERLAY_HEIGHT_TILES)

/**
 * @brief Add the overlay window to a BG layer. Only one overlay exists at a time.
 *
 * The window uses SEED_OVERLAY_TILE_COUNT tiles starting at baseTile, and text
 * colors 1 (letter), 2 (shadow) and 15 (background) of the given palette.
 */
void SeedOverlay_Init(BgConfig *bgConfig, enum BgLayer bgLayer, u8 palette, u16 baseTile, enum HeapID heapID);

/**
 * @brief Load a black-on-white palette for the overlay into the given BG palette.
 */
void SeedOverlay_LoadPalette(enum BgLayer bgLayer, u8 palette);

/**
 * @brief Redraw the overlay if the seed changed. Call once per frame.
 */
void SeedOverlay_Update(void);

/**
 * @brief Draw an arbitrary value right away, without waiting for VBlank.
 *
 * For screens that don't run scheduled BG updates, or that hide the real seed.
 */
void SeedOverlay_Draw(u32 value);

/**
 * @brief Force a full redraw on the next update. Use after something else overwrites the overlay's tiles.
 */
void SeedOverlay_Invalidate(void);

void SeedOverlay_Free(void);

#endif // POKEPLATINUM_SEED_OVERLAY_H
