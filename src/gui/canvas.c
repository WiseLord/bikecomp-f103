#include "canvas.h"

#include <inttypes.h>
#include <stdio.h>

#include "comp.h"
#include "font7seg.h"

static Canvas canvas;

void canvasInit()
{
    glcdInit(GLCD_LANDSCAPE_ROT);
    glcdSetIdle(true);

    canvas.glcd = glcdGet();

    PalIdx palIdx = PAL_DEFAULT;
    paletteSetIndex(palIdx);
    canvas.pal = paletteGet();

    glcdDrawRect(0, 0, dispdrv.width, dispdrv.height, canvas.pal->bg);
}

Canvas *canvasGet()
{
    return &canvas;
}

void canvasClear()
{
    GlcdRect rect = canvas.glcd->rect;

    glcdDrawRect(0, 0, rect.w, rect.h, canvas.pal->bg);
    glcdShift(0);

    glcdSetFontColor(canvas.pal->fg);
    glcdSetFontBgColor(canvas.pal->bg);
}

void canvasShowComp(bool clear)
{
    const Palette *pal = paletteGet();
    Comp *comp = compGet();

    font7segLoad(font_7seg_4);

    glcdSetFontColor(pal->fg);
    glcdSetFontBgColor(pal->bg);

    glcdSetXY(100, 100);

    char buf[8];
    snprintf(buf, sizeof(buf), "%05" PRId32, comp->wTurns);

    font7segWriteString(buf);

    utilmDelay(100);

}
