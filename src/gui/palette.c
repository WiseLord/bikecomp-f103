#include "palette.h"

static PalIdx palIdx = PAL_DEFAULT;

static const Palette palette[PAL_END] = {
    [PAL_DEFAULT] = {
        .fg = COLOR_BLACK,
        .bg = COLOR_WHITE,
    },
};

const Palette *paletteGet(void)
{
    return &palette[palIdx];
}

PalIdx paletteGetIndex(void)
{
    return palIdx;
}

void paletteSetIndex(PalIdx index)
{
    palIdx = index;
}
