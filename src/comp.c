#include "comp.h"

#include "display/glcd.h"
#include "display/font7seg.h"
#include "pins.h"

void compInit()
{
    pinsInit();
    glcdInit(GLCD_LANDSCAPE);
}

void compRun()
{
    // Graphics
    int16_t w = glcdGet()->rect.w;
    int16_t h = glcdGet()->rect.h;

    glcdDrawRect(0, 0, w, h, COLOR_BLACK);

    if (h >= 240) {
        glcdSetFont(&fontterminus32);
    } else if (h >= 176) {
        glcdSetFont(&fontterminus24);
    } else {
        glcdSetFont(&fontterminus16);
    }
    font7segLoad(font_7seg_4);

    int16_t tw = w / 16;
    int16_t th = h / 4;

    glcdDrawRect(w / 2 + tw * 1, h / 8 * 5, tw / 4 * 6, th, COLOR_RED);
    glcdDrawRect(w / 2 + tw * 3, h / 8 * 5, tw / 4 * 6, th, COLOR_LIME);
    glcdDrawRect(w / 2 + tw * 5, h / 8 * 5, tw / 4 * 6, th, COLOR_BLUE);

    int16_t rx = w / 4 * 3;
    int16_t ry = h / 4 * 1;

    glcdDrawRing(rx, ry, ry - 2, 3, COLOR_WHITE);
    glcdFbSync();


    while (1) {

        LL_mDelay(500);
    }
}
