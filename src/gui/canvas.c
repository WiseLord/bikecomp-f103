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

static void drawTime(void)
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

static void drawWheelAnimation(void)
{
    const tImage *img;

    uint8_t phase = compGet()->wTurns % 16;

    glcdSetXY(0, 0);
    img = glcdFindIcon(ICON_WHEEL_BEGIN + phase, &bikecompicons);
    glcdDrawImage(img, canvas.pal->fg, canvas.pal->bg);
}

static void drawSpeed(void)
{
    char buf[8];

    int32_t param = compGetSpeedMph();

    glcdSetXY(0, 0);
    font7segLoad(font_7seg_10);
    snprintf(buf, sizeof(buf), "%2d", (int)(param / 1000));
    font7segWriteString(buf);

    glcdSetY(3);
    font7segLoad(font_7seg_7);
    snprintf(buf, sizeof(buf), ".%1d", (int)(param % 1000 / 100));
    font7segWriteString(buf);

    glcdSetXY(glcdGet()->x - 56, 70);
    glcdSetFont(&fontterminus24b);
    snprintf(buf, sizeof(buf), "%s", "km/h");
    glcdWriteString(buf);
}

static void drawSpeedIcons(bool clear)
{
    const tImage *img;

    static bool iconAboveOld = false;
    static bool iconBelowOld = false;

    bool iconAbove = false;
    bool iconBelow = false;

    int32_t speed = compGetSpeedMph();
    int32_t avgSpeed = compGetAvgSpeedMph();

    if (avgSpeed != 0 && speed != 0) {
        iconAbove = (avgSpeed - speed < 1000);
        iconBelow = (speed - avgSpeed < 1000);
    }

    glcdSetXY(0, 0);
    img = glcdFindIcon(ICON_ABOVE, &bikecompicons);

    if (clear || iconAbove != iconAboveOld) {
        if (iconAbove) {
            glcdDrawImage(img, canvas.pal->fg, canvas.pal->bg);
        } else {
            glcdDrawImage(img, canvas.pal->bg, canvas.pal->bg);
        }
    }

    glcdSetXY(0, 30);
    img = glcdFindIcon(ICON_BELOW, &bikecompicons);

    if (clear || iconBelow != iconBelowOld) {
        if (iconBelow) {
            glcdDrawImage(img, canvas.pal->fg, canvas.pal->bg);
        } else {
            glcdDrawImage(img, canvas.pal->bg, canvas.pal->bg);
        }
    }

    iconAboveOld = iconAbove;
    iconBelowOld = iconBelow;
}

static void drawTrackLen()
{
    char buf[32];

    int32_t param = compGetTrackLengthM();

    glcdSetXY(4, 0);
    glcdSetFont(&fontterminus24b);
    snprintf(buf, sizeof(buf), "%s", "Track length");
    glcdWriteString(buf);

    font7segLoad(font_7seg_6);
    glcdSetXY(76, 28);
    snprintf(buf, sizeof(buf), "%3d", (int)(param / 1000));
    font7segWriteString(buf);

    font7segLoad(font_7seg_4);
    snprintf(buf, sizeof(buf), ".%02d", (int)(param % 1000 / 10));
    font7segWriteString(buf);
}

static void drawTrackTime()
{
    char buf[32];

    int32_t param = compGetTrackTime();

    int8_t seconds = param % 60;
    param /= 60;
    int8_t minutes = param % 60;
    param /= 60;
    int8_t hours = param % 24;
    param /= 24;

    // TODO: selecte between HH:MM:ss and DD:HH:mm

    glcdSetXY(4, 0);
    glcdSetFont(&fontterminus24b);
    snprintf(buf, sizeof(buf), "%s", "Track time");
    glcdWriteString(buf);

    font7segLoad(font_7seg_6);
    glcdSetXY(28, 28);
    snprintf(buf, sizeof(buf), "%2d:%02d", hours, minutes);
    font7segWriteString(buf);

    font7segLoad(font_7seg_4);
    snprintf(buf, sizeof(buf), ".%02d", seconds);

    font7segWriteString(buf);
}

static void drawAvgSpeed()
{
    char buf[32];

    int32_t param = compGetAvgSpeedMph();

    glcdSetXY(4, 0);
    glcdSetFont(&fontterminus24b);
    snprintf(buf, sizeof(buf), "%s", "Average speed");
    glcdWriteString(buf);

    font7segLoad(font_7seg_6);
    glcdSetXY(76, 28);
    snprintf(buf, sizeof(buf), "%3d", (int)(param / 1000));
    font7segWriteString(buf);

    font7segLoad(font_7seg_4);
    snprintf(buf, sizeof(buf), ".%02d", (int)(param % 1000 / 10));
    font7segWriteString(buf);
}

static void drawCadence()
{
    char buf[32];

    int32_t param = compGetCadenceRP10M();

    glcdSetXY(4, 0);
    glcdSetFont(&fontterminus24b);
    snprintf(buf, sizeof(buf), "%s", "Cadence");
    glcdWriteString(buf);

    font7segLoad(font_7seg_6);
    glcdSetXY(100, 28);
    snprintf(buf, sizeof(buf), "%3d", (int)(param / 10));
    font7segWriteString(buf);

    font7segLoad(font_7seg_4);
    snprintf(buf, sizeof(buf), ".%1d", (int)(param % 10));
    font7segWriteString(buf);
}

static void drawDistance()
{
    char buf[32];

    int32_t param = compGetTotalDistanceM();

    glcdSetXY(4, 0);
    glcdSetFont(&fontterminus24b);
    snprintf(buf, sizeof(buf), "%s", "Total distance");
    glcdWriteString(buf);

    font7segLoad(font_7seg_6);
    glcdSetXY(28, 28);
    snprintf(buf, sizeof(buf), "%5d", (int)(param / 1000));
    font7segWriteString(buf);

    font7segLoad(font_7seg_4);
    snprintf(buf, sizeof(buf), ".%1d", (int)(param % 1000 / 100));
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
    case BIKEPAR_CADENCE:
        drawCadence();
        break;
    case BIKEPAR_DISTANCE:
        drawDistance();
        break;
    default:
        break;
    }
}

static void drawDivider(int16_t y, bool clear)
{
    if (clear) {
        glcdResetRect();
        glcdDrawRect(4, y, 232, 2, COLOR_BLACK);
    }
}

void canvasShowMain(bool clear)
{
    const Palette *pal = paletteGet();
    Comp *comp = compGet();

    glcdResetRect();
    glcdSetFontColor(pal->fg);
    glcdSetFontBgColor(pal->bg);

//    glcdDrawFrame(0, 0, 240, 320, 4, COLOR_RED);

    glcdSetRectValues(4, 4, 75, 27);
    drawTime();

    glcdSetRectValues(100, 4, 27, 27);
    drawWheelAnimation();

    drawDivider(35, clear);

    glcdSetRectValues(67, 41, 169, 90);
    drawSpeed();

    glcdSetRectValues(15, 57, 24, 54);
    drawSpeedIcons(clear);

    drawDivider(135, clear);

    // Parameter 1
    glcdSetRectValues(0, 141, 240, 82);
    static BikePar par1 = BIKEPAR_END;
    if (!clear && par1 != comp->par1) {
        canvasClear();
    }
    par1 = comp->par1;
    drawBikeParam(par1);

    drawDivider(228, clear);

    // Parameter 2
    glcdSetRectValues(0, 234, 240, 82);
    static BikePar par2 = BIKEPAR_END;
    if (!clear && par2 != comp->par2) {
        canvasClear();
    }
    par2 = comp->par2;
    drawBikeParam(par2);
}
