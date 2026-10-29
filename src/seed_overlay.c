#include "seed_overlay.h"

#include <nitro.h>

#include "constants/charcode.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "math_util.h"
#include "string_gf.h"
#include "text.h"

#define SEED_OVERLAY_COLOR_BG 15

static Window sWindow;
static String *sString;
static u32 sShownSeed;
static BOOL sActive = FALSE;
static BOOL sInvalidated = FALSE;

void SeedOverlay_Init(BgConfig *bgConfig, enum BgLayer bgLayer, u8 palette, u16 baseTile, enum HeapID heapID)
{
    Window_Add(bgConfig, &sWindow, bgLayer, 0, 0, SEED_OVERLAY_WIDTH_TILES, SEED_OVERLAY_HEIGHT_TILES, palette, baseTile);
    sString = String_Init(16, heapID);
    sShownSeed = 0;
    sActive = TRUE;

    Window_FillTilemap(&sWindow, SEED_OVERLAY_COLOR_BG);
    Window_CopyToVRAM(&sWindow);
}

void SeedOverlay_LoadPalette(enum BgLayer bgLayer, u8 palette)
{
    static const u16 colors[16] = {
        [0] = GX_RGB(0, 0, 0),
        [1] = GX_RGB(0, 0, 0),
        [2] = GX_RGB(15, 15, 15),
        [SEED_OVERLAY_COLOR_BG] = GX_RGB(31, 31, 31),
    };

    Bg_LoadPalette(bgLayer, (void *)colors, sizeof(colors), PLTT_OFFSET(palette));
}

void SeedOverlay_Update(void)
{
    u32 seed = LCRNG_GetSeed();

    if (!sActive || (seed == sShownSeed && !sInvalidated)) {
        return;
    }

    sInvalidated = FALSE;

    String_Clear(sString);

    for (int shift = 28; shift >= 0; shift -= 4) {
        u32 nibble = (seed >> shift) & 0xF;
        String_AppendChar(sString, nibble < 10 ? CHAR_0 + nibble : CHAR_A + (nibble - 10));
    }

    Window_FillTilemap(&sWindow, SEED_OVERLAY_COLOR_BG);
    Text_AddPrinterWithParamsAndColor(&sWindow, FONT_SYSTEM, sString, 0, 0, TEXT_SPEED_INSTANT, TEXT_COLOR(1, 2, SEED_OVERLAY_COLOR_BG), NULL);
    Window_ScheduleCopyToVRAM(&sWindow);
    sShownSeed = seed;
}

void SeedOverlay_Invalidate(void)
{
    sInvalidated = TRUE;
}

void SeedOverlay_Free(void)
{
    if (!sActive) {
        return;
    }

    sActive = FALSE;
    Window_Remove(&sWindow);
    String_Free(sString);
}
