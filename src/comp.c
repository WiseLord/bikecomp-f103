#include "comp.h"

#include "gui/canvas.h"
#include "hwlibs.h"
#include "input.h"
#include "swtimers.h"
#include "timers.h"

#define ANTIBOUNCE  100

static CompPriv priv;
static Comp comp = { .priv = &priv };

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

    comp.wLenMm = 2062; // TODO: Read from settings

    NVIC_SetPriority(EXTI9_5_IRQn, 0);
    NVIC_EnableIRQ(EXTI9_5_IRQn);

    timerInit(TIM_COMP, 7199, 65535);   // 10k counts/sec, reset after 6.5 seconds
    swTimInit();

    swTimSet(SW_TIM_WHEEL_ANTIBOUNCE, SW_TIM_ON);
    swTimSet(SW_TIM_PEDAL_ANTIBOUNCE, SW_TIM_ON);
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

uint32_t compGetSpeedMph(void)
{
    /*
     * Counter freq = 10000 counts / 1 sec = wCntLastTurn / time(sec)
     * So, time(sec) = wCntLastTurn / 10000;
     * Speed(mm/sec) = wLenMm / time(sec) = wLenMm * 10000 / wCntLastTurn
     * Speed(m/h) = Speed(mm/sec) * 3600(sec/h) / 1000(mm/m), so
     * Speed(m/h) = (wLenMm * 10000 / wCntLastTurn) * 3600 / 1000
     * Speed(m/h) = wLenMm * 36000 / wCntLastTurn
     */

    uint32_t speedMph = comp.wLenMm * 36000 / priv.wCntLastTurn;

    return speedMph;
}


static void compWheelCb(void)
{
    if (swTimGet(SW_TIM_WHEEL_ANTIBOUNCE > 0)) {
        return;
    }

    uint32_t tcnt = LL_TIM_GetCounter(TIM_COMP);
    LL_TIM_SetCounter(TIM_COMP, 0);

    // Update counter for the pedal
    priv.pCntCurrTurn += tcnt;

    // Save wheel counter of the full turn
    priv.wCntLastTurn = priv.wCntCurrTurn + tcnt;
    priv.wCntCurrTurn = 0;

    swTimSet(SW_TIM_WHEEL_ANTIBOUNCE, ANTIBOUNCE);

    comp.wTurns++;
}

static void compPedalCb(void)
{
    if (swTimGet(SW_TIM_PEDAL_ANTIBOUNCE) > 0) {
        return;
    }

    uint32_t tcnt = LL_TIM_GetCounter(TIM_COMP);
    LL_TIM_SetCounter(TIM_COMP, 0);

    // Update counter for the wheel
    priv.wCntCurrTurn += tcnt;

    // Save pedal counter of the full turn
    priv.pCntLastTurn = priv.pCntCurrTurn + tcnt;
    priv.pCntCurrTurn = 0;

    swTimSet(SW_TIM_PEDAL_ANTIBOUNCE, ANTIBOUNCE);

    comp.pTurns++;
}

static void compTimCb(void)
{
    comp.wTurns = 0;
    comp.pTurns = 0;
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

void TIM_COMP_HANDLER(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM_COMP)) {
        // Clear the update interrupt flag
        LL_TIM_ClearFlag_UPDATE(TIM_COMP);

        compTimCb();
    }
}
