#include "comp.h"

#include "display/glcd.h"
#include "gui/font7seg.h"

#include <stdio.h>

void compInit()
{
    glcdInit(GLCD_LANDSCAPE_ROT);
    glcdSetIdle(true);
}

void compRun()
{
    int i = 0;

    // Graphics
    int16_t w = glcdGet()->rect.w;
    int16_t h = glcdGet()->rect.h;

    font7segLoad(font_7seg_4);

    glcdDrawRect(0, 0, w, h, COLOR_WHITE);
    glcdSetFontColor(COLOR_BLACK);
    glcdSetFontBgColor(COLOR_WHITE);

    while (1) {
        if (i % 100 == 50) {
            glcdDrawRect(0, 0, w, h, COLOR_WHITE);
            glcdSetFontColor(COLOR_BLACK);
            glcdSetFontBgColor(COLOR_WHITE);
        }
        if (i % 100 == 0) {
            glcdDrawRect(0, 0, w, h, COLOR_BLACK);
            glcdSetFontColor(COLOR_WHITE);
            glcdSetFontBgColor(COLOR_BLACK);
        }

        if (i % 50 == 25) {
            glcdSleep(true);
        }
        if (i % 50 == 0) {
            glcdSleep(false);
        }

        glcdSetXY(100, 100);

        char buf[8];
        snprintf(buf, sizeof(buf), "%05d", i);

        font7segWriteString(buf);

        utilmDelay(100);
        i++;
    }
}
