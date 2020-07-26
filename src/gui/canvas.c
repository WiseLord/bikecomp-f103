#include "canvas.h"

#include <inttypes.h>
#include <stdio.h>

#include "comp.h"
#include "input.h"
#include "font7seg.h"
#include "rtc.h"
#include "swtimers.h"

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
    int16_t y = 60;

    char buf[8];

    Comp *comp = compGet();

    font7segLoad(font_7seg_3);

    glcdSetXY(x, y);
    snprintf(buf, sizeof(buf), "%5" PRId32, comp->wTurns);
    font7segWriteString(buf);

    glcdSetXY(x, y + 40);
    snprintf(buf, sizeof(buf), "%5" PRId32, LL_TIM_GetCounter(TIM_COMP));
    font7segWriteString(buf);

    glcdSetXY(x, y + 80);
    snprintf(buf, sizeof(buf), "%5" PRId32, comp->priv->wCntLastTurn);
    font7segWriteString(buf);

    glcdSetXY(x, y + 120);
    snprintf(buf, sizeof(buf), "%5" PRId32, comp->priv->pCntLastTurn);
    font7segWriteString(buf);

    glcdSetXY(x, y + 120);
    snprintf(buf, sizeof(buf), "%5" PRId32, swTimGet(SW_TIM_RTC_INIT));
    font7segWriteString(buf);
}

static void drawSpeed(void)
{
    int16_t x = 82;
    int16_t y = 1;

    char buf[8];

    uint32_t speedMph = compGetSpeedMph();

    font7segLoad(font_7seg_10);
    glcdSetXY(x, y);
    snprintf(buf, sizeof(buf), "%2u", (unsigned)(speedMph / 1000));
    font7segWriteString(buf);

    font7segLoad(font_7seg_7);
    glcdSetXY(x + 122, y + 27);
    snprintf(buf, sizeof(buf), "%1u", (unsigned)(speedMph % 1000 / 100));
    font7segWriteString(buf);
}



void drawTime(void)
{
    int16_t x = 70;
    int16_t y = 280;

    RTC_type rtc;
    char buf[10];

    rtcGetTime(&rtc);

    font7segLoad(font_7seg_4);
    glcdSetXY(x, y);
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", rtc.hour, rtc.min, rtc.sec);

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
//        glcdDrawRect(5, 178, 230, 2, COLOR_BLACK);
    }

    drawTest();

    drawTime();
//    drawTrackLen();
//    drawTrackTime();
}
