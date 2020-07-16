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
    int16_t x = 160;
    int16_t y = 5;

    font7segLoad(font_7seg_10);
    glcdSetXY(x, y);

    char buf[8];

    snprintf(buf, sizeof(buf), "%02d", 23);
    font7segWriteString(buf);

    glcdSetXY(x + 122, y + 27);
    font7segLoad(font_7seg_7);
    font7segWriteString("2");
}

void drawTrackLen()
{
    int16_t x = 170;
    int16_t y = 120;

    char buf[8];

    font7segLoad(font_7seg_5);
    glcdSetXY(x, y);
    snprintf(buf, sizeof(buf), "%03d", 328);
    font7segWriteString(buf);

    font7segLoad(font_7seg_3);
    glcdSetXY(x + 90, y + 18);
    snprintf(buf, sizeof(buf), ".%02d", 35);

    font7segWriteString(buf);
}

void drawTrackTime()
{
    char buf[8];
    int16_t x = 145;
    int16_t y = 190;

    font7segLoad(font_7seg_5);
    glcdSetXY(x, y);
    snprintf(buf, sizeof(buf), "%02d:%02d", 32, 26);
    font7segWriteString(buf);

    font7segLoad(font_7seg_3);
    glcdSetXY(x + 130, y + 18);
    snprintf(buf, sizeof(buf), ".%02d", 33);

    font7segWriteString(buf);
}

void canvasShowComp(bool clear)
{
    const Palette *pal = paletteGet();
    Comp *comp = compGet();

    glcdSetFontColor(pal->fg);
    glcdSetFontBgColor(pal->bg);

//    glcdDrawRect(5, 60, 310, 2, COLOR_BLACK);
    glcdDrawRect(5, 178, 310, 2, COLOR_BLACK);

//    glcdDrawFrame(5, 75, 80, 30, 5, COLOR_BLACK);

//    glcdDrawRing(30, 140, 25, 5, COLOR_BLACK);

    drawTime();
    drawSpeed();
    drawTrackLen();
    drawTrackTime();

    return;


    font7segLoad(font_7seg_4);
    glcdSetXY(100, 100);

    char buf[8];
    snprintf(buf, sizeof(buf), "%05" PRId32, comp->wTurns);

    font7segWriteString(buf);

    utilmDelay(100);

}
