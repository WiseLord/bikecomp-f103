#include "comp.h"

#include "gui/canvas.h"
#include "hwlibs.h"
#include "input.h"

static Comp comp;

static void compActionGet(void);
static void compActionHandle(void);
static void compScreenShow(void);

static void compInitPins(void)
{
    LL_GPIO_InitTypeDef initDef = {0};

    initDef.Mode = LL_GPIO_MODE_INPUT;
    initDef.Pull = LL_GPIO_PULL_UP;

    initDef.Pin = WHEEL_Pin;
    LL_GPIO_Init(WHEEL_Port, &initDef);
    initDef.Pin = PEDAL_Pin;
    LL_GPIO_Init(PEDAL_Port, &initDef);

    LL_GPIO_AF_SetEXTISource(WHEEL_AR_ExtiPort, WHEEL_AR_ExtiLine);
    LL_EXTI_DisableEvent_0_31(WHEEL_ExtiLine);
    LL_EXTI_EnableIT_0_31(WHEEL_ExtiLine);
    LL_EXTI_EnableRisingTrig_0_31(WHEEL_ExtiLine);

    LL_GPIO_AF_SetEXTISource(PEDAL_AR_ExtiPort, PEDAL_AR_ExtiLine);
    LL_EXTI_DisableEvent_0_31(PEDAL_ExtiLine);
    LL_EXTI_EnableIT_0_31(PEDAL_ExtiLine);
    LL_EXTI_EnableRisingTrig_0_31(PEDAL_ExtiLine);
}

void compInit()
{
    compInitPins();

    canvasInit();
    canvasClear();

    inputInit();

    NVIC_SetPriority(EXTI9_5_IRQn, 0);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
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

void compWheelCb(void)
{
    comp.wTurns++;
    // TODO: antibounce
}

void compPedalCb(void)
{
    comp.pTurns++;
    // TODO: antibounce
}

void EXTI_COMP_HANDLER()
{
    if (LL_EXTI_IsActiveFlag_0_31(WHEEL_ExtiLine) != RESET) {
        // Clear RC line interrupt
        LL_EXTI_ClearFlag_0_31(WHEEL_ExtiLine);

        // Callback
        compWheelCb();
    }
    if (LL_EXTI_IsActiveFlag_0_31(PEDAL_ExtiLine) != RESET) {
        // Clear RC line interrupt
        LL_EXTI_ClearFlag_0_31(PEDAL_ExtiLine);

        // Callback
        compPedalCb();
    }
}
