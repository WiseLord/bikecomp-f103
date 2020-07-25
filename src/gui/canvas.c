#include "canvas.h"

#include <inttypes.h>
#include <stdio.h>

#include "comp.h"
#include "input.h"
#include "font7seg.h"

static Canvas canvas;

void canvasInit()
{
    glcdInit(GLCD_PORTRATE);
    glcdSetIdle(true);

    canvas.glcd = glcdGet();

    PalIdx palIdx = PAL_DEFAULT;
    paletteSetIndex(palIdx);
    canvas.pal = paletteGet();
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

static void drawTest(void)
{
    int16_t x = 5;
    int16_t y = 100;

    char buf[8];

    font7segLoad(font_7seg_3);

    glcdSetXY(x, y);
    snprintf(buf, sizeof(buf), "%5" PRId32, compGet()->wTurns);
    font7segWriteString(buf);

    glcdSetXY(x, y + 40);
    snprintf(buf, sizeof(buf), "%5" PRId32, compGet()->pTurns);
    font7segWriteString(buf);
}

static void drawSpeed(void)
{
    int16_t x = 82;
    int16_t y = 1;

    font7segLoad(font_7seg_10);
    glcdSetXY(x, y);

    char buf[8];

    snprintf(buf, sizeof(buf), "%02d", 23);
    font7segWriteString(buf);

    glcdSetXY(x + 122, y + 27);
    font7segLoad(font_7seg_7);
    font7segWriteString("2");
}



void drawTime(void)
{
    font7segLoad(font_7seg_5);

    glcdSetXY(70, 250);

    char buf[8];
    snprintf(buf, sizeof(buf), "%02d:%02d", 21, 30);

    font7segWriteString(buf);
}

void drawTrackLen()
{
    int16_t x = 100;
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
    int16_t x = 55;
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

    glcdSetFontColor(pal->fg);
    glcdSetFontBgColor(pal->bg);

    drawSpeed();

    if (clear) {
        glcdDrawRect(5, 178, 230, 2, COLOR_BLACK);
    }

    drawTest();

    drawTime();
    drawTrackLen();
    drawTrackTime();
}
