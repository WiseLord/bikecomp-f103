#include "canvas.h"

#include <inttypes.h>
#include <stdio.h>

#include "comp.h"
#include "icons.h"
#include "input.h"
#include "font7seg.h"
#include "rtc.h"
#include "swtimers.h"

static Canvas canvas;

static const tImage *glcdFindIcon(Icon code, const tFont *iFont)
{
    const tImage *ret = NULL;

    // Find icon pos
    int32_t iPos = -1;
    for (int16_t i = 0; i < iFont->length; i++) {
        if (iFont->chars[i].code == (int32_t)code) {
            iPos = i;
            break;
        }
    }

    if (iPos >= 0) {
        ret = iFont->chars[iPos].image;
    }

    return  ret;
}

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

void drawTime(bool clear)
{
    char buf[10];

    RTC_type rtc;
    rtcGetTime(&rtc);

    glcdSetXY(0, 0);
    font7segLoad(font_7seg_3);

    snprintf(buf, sizeof(buf), "%02d", rtc.hour);
    font7segWriteString(buf);

    glcdSetFontColor(rtc.sec % 2 ? paletteGet()->bg : paletteGet()->fg);
    font7segWriteChar(':');
    glcdSetFontColor(paletteGet()->fg);

    snprintf(buf, sizeof(buf), "%02d", rtc.min);
    font7segWriteString(buf);
}

static void drawSpeed(bool clear)
{
    char buf[8];

    int32_t speedMph = compGetSpeedMph();

    glcdSetXY(0, 0);
    font7segLoad(font_7seg_10);
    snprintf(buf, sizeof(buf), "%2d", (int)(speedMph / 1000));
    font7segWriteString(buf);

    glcdSetY(3);
    font7segLoad(font_7seg_7);
    snprintf(buf, sizeof(buf), ".%1d", (int)(speedMph % 1000 / 100));
    font7segWriteString(buf);

    glcdSetXY(glcdGet()->x - 56, 70);
    glcdSetFont(&fontterminus24b);
    snprintf(buf, sizeof(buf), "%s", "km/h");
    glcdWriteString(buf);
}

static void drawSpeedIcons(bool clear)
{
    const tImage *img;

    glcdSetXY(0, 0);
    img = glcdFindIcon(ICON_ABOVE, &bikecompicons);
    glcdDrawImage(img, canvas.pal->fg, canvas.pal->bg);

    glcdSetXY(0, 30);
    img = glcdFindIcon(ICON_BELOW, &bikecompicons);
    glcdDrawImage(img, canvas.pal->fg, canvas.pal->bg);
}

static void drawTrackLen()
{
    char buf[32];

    int32_t trackLengthM = compGetAvgSpeedMph();

    glcdSetXY(3, 0);
    glcdSetFont(&fontterminus24b);
    snprintf(buf, sizeof(buf), "%s", "Track length");
    glcdWriteString(buf);

    font7segLoad(font_7seg_6);
    glcdSetXY(75, 28);
    snprintf(buf, sizeof(buf), "%3d", (int)(trackLengthM / 1000));
    font7segWriteString(buf);

    font7segLoad(font_7seg_4);
    snprintf(buf, sizeof(buf), ".%02d", (int)(trackLengthM % 1000 / 10));
    font7segWriteString(buf);
}

static void drawAvgSpeed()
{
    char buf[32];

    int32_t trackLengthM = compGetAvgSpeedMph();

    glcdSetXY(3, 0);
    glcdSetFont(&fontterminus24b);
    snprintf(buf, sizeof(buf), "%s", "Track length");
    glcdWriteString(buf);

    font7segLoad(font_7seg_6);
    glcdSetXY(75, 28);
    snprintf(buf, sizeof(buf), "%3d", (int)(trackLengthM / 1000));
    font7segWriteString(buf);

    font7segLoad(font_7seg_4);
    snprintf(buf, sizeof(buf), ".%02d", (int)(trackLengthM % 1000 / 10));
    font7segWriteString(buf);
}

static void drawTrackTime()
{
    char buf[32];

    int32_t trackTime = compGetTrackTime();

    int8_t seconds = trackTime % 60;
    trackTime /= 60;
    int8_t minutes = trackTime % 60;
    trackTime /= 60;
    int8_t hours = trackTime % 24;
    trackTime /= 24;

    // TODO: selecte between HH:MM:ss and DD:HH:mm

    glcdSetXY(3, 0);
    glcdSetFont(&fontterminus24b);
    snprintf(buf, sizeof(buf), "%s", "Track time");
    glcdWriteString(buf);

    font7segLoad(font_7seg_6);
    glcdSetXY(26, 28);
    snprintf(buf, sizeof(buf), "%2d:%02d", hours, minutes);
    font7segWriteString(buf);

    font7segLoad(font_7seg_4);
    snprintf(buf, sizeof(buf), ".%02d", seconds);

    font7segWriteString(buf);
}

static void drawBikeParam(BikePar param)
{
    switch (param) {
    case BIKEPAR_TRACK:
        drawTrackLen();
        break;
    case BIKEPAR_TRACK_TIME:
        drawTrackTime();
        break;
    case BIKEPAR_SPEED_AVG:
        drawAvgSpeed();
        break;
    default:
        break;
    }
}

void canvasShowMain(bool clear)
{
    const Palette *pal = paletteGet();
    Comp *comp = compGet();

    glcdResetRect();
    glcdSetFontColor(pal->fg);
    glcdSetFontBgColor(pal->bg);


    glcdSetRectValues(4, 4, 75, 27);
//    glcdDrawRect(0, 0, glcdGet()->rect.w, glcdGet()->rect.h, COLOR_RED);
    drawTime(clear);

    if (clear) {
        glcdResetRect();
        glcdDrawRect(3, 35, 234, 2, COLOR_BLACK);
    }

    glcdSetRectValues(67, 41, 169, 90);
//    glcdDrawRect(0, 0, glcdGet()->rect.w, glcdGet()->rect.h, COLOR_RED);
    drawSpeed(clear);

    glcdSetRectValues(15, 57, 24, 54);
    drawSpeedIcons(clear);

    if (clear) {
        glcdResetRect();
        glcdDrawRect(3, 135, 234, 2, COLOR_BLACK);
    }

    glcdSetRectValues(0, 141, 240, 82);
//    glcdDrawRect(0, 0, glcdGet()->rect.w, glcdGet()->rect.h, COLOR_RED);

    drawBikeParam(comp->par1);

    if (clear) {
        glcdResetRect();
        glcdDrawRect(3, 228, 234, 2, COLOR_BLACK);
    }

    glcdSetRectValues(0, 234, 240, 82);
//    glcdDrawRect(0, 0, glcdGet()->rect.w, glcdGet()->rect.h, COLOR_RED);

    drawBikeParam(comp->par2);
}
