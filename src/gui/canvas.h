#ifndef CANVAS_H
#define CANVAS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "display/glcd.h"
#include "palette.h"

typedef struct {
    Glcd *glcd;
    const Palette *pal;
} Canvas;

void canvasInit(void);
Canvas *canvasGet(void);

void canvasClear(void);

void canvasShowMain(bool clear);

#ifdef __cplusplus
}
#endif


#endif // CANVAS_H
