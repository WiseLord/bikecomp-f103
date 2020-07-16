#include "canvas.h"

#include <inttypes.h>
#include <stdio.h>

#include "comp.h"
#include "font7seg.h"

static Canvas canvas;

void canvasInit()
{
    glcdInit(GLCD_LANDSCAPE);
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

void drawTime(void)
{
    font7segLoad(font_7seg_5);

    glcdSetXY(5, 8);

    char buf[8];
    snprintf(buf, sizeof(buf), "%02d:%02d", 21, 30);

    font7segWriteString(buf);
}

void drawSpeed(void)
{
    int16_t x = 130;
    int16_t y = 71;

    font7segLoad(font_7seg_11);
    glcdSetXY(x, y);

    char buf[8];

    snprintf(buf, sizeof(buf), "%02d", 23);
    font7segWriteString(buf);

    glcdSetXY(x + 140, y + 25);
    font7segLoad(font_7seg_8);
    font7segWriteString("2");
}


void canvasShowComp(bool clear)
{
    const Palette *pal = paletteGet();
    Comp *comp = compGet();

    glcdSetFontColor(pal->fg);
    glcdSetFontBgColor(pal->bg);

    glcdDrawRect(5, 60, 310, 2, COLOR_BLACK);
    glcdDrawRect(5, 178, 310, 2, COLOR_BLACK);

    glcdDrawFrame(5, 75, 80, 30, 5, COLOR_BLACK);

    glcdDrawRing(30, 140, 25, 5, COLOR_BLACK);

    drawTime();

    drawSpeed();

    return;


    font7segLoad(font_7seg_4);
    glcdSetXY(100, 100);

    char buf[8];
    snprintf(buf, sizeof(buf), "%05" PRId32, comp->wTurns);

    font7segWriteString(buf);

    utilmDelay(100);

}
