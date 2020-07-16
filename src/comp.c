#include "comp.h"

#include "gui/canvas.h"

static Comp comp;

static void compActionGet(void);
static void compActionHandle(void);
static void compScreenShow(void);

void compInit()
{
    canvasInit();
}

Comp *compGet()
{
    return &comp;
}

static void compActionGet(void)
{

}

static void compActionHandle(void)
{
    comp.wTurns++;
}

static void compScreenShow(void)
{
    canvasShowComp(true);
}

void compRun()
{
    while (1) {
        compActionGet();
        compActionHandle();
        compScreenShow();
    }
}
