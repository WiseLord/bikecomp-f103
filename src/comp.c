#include "comp.h"

#include "action.h"
#include "gui/canvas.h"
#include "hwlibs.h"
#include "input.h"
#include "rtc.h"
#include "settings.h"
#include "swtimers.h"
#include "timers.h"

#define ANTIBOUNCE  100

#define FLAG_EXIT           0
#define FLAG_ENTER          1
#define FLAG_SWITCH         2

static CompPriv priv;
static Comp comp = { .priv = &priv };

static Action action = {
    .type = ACTION_NONE,
    .value = FLAG_ENTER,
};

static void actionGetButtons(void);
static void actionGetTimers(void);

static void compActionGet(void);
static void compActionHandle(void);
static void compScreenShow(void);

static void actionSet(ActionType type, int16_t value)
{
    action.type = type;
    action.value = value;
}

static void actionGetButtons(void)
{
    CmdBtn cmdBtn = inputGetBtnCmd();

    if (cmdBtn.btn) {
        if (cmdBtn.flags & BTN_FLAG_LONG_PRESS) {
            actionSet(ACTION_BTN_LONG, (int16_t)cmdBtn.btn);
        } else {
            actionSet(ACTION_BTN_SHORT, (int16_t)cmdBtn.btn);
        }
    }
}
static void actionGetTimers(void)
{
    if (swTimGet(SW_TIM_DISPLAY) == 0) {
        actionSet(ACTION_DISP_EXPIRED, 0);
    } else if (swTimGet(SW_TIM_RTC_INIT) == 0) {
        actionSet(ACTION_INIT_RTC, 0);
    }
}

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
    LL_EXTI_DisableRisingTrig_0_31(WHEEL_ExtiLine);
    LL_EXTI_EnableFallingTrig_0_31(WHEEL_ExtiLine);

    LL_GPIO_AF_SetEXTISource(PEDAL_AR_ExtiPort, PEDAL_AR_ExtiLine);
    LL_EXTI_DisableEvent_0_31(PEDAL_ExtiLine);
    LL_EXTI_EnableIT_0_31(PEDAL_ExtiLine);
    LL_EXTI_DisableRisingTrig_0_31(PEDAL_ExtiLine);
    LL_EXTI_EnableFallingTrig_0_31(PEDAL_ExtiLine);
}

static void compSecondsCb(void)
{
    if (comp.inMove) {
        comp.trackTime++;
    }
}

void compInit()
{
    settingsInit();

    compInitPins();

    canvasInit();
    canvasClear();

    inputInit();

    comp.wLenMm = settingsGet(PARAM_BIKE_WHEEL_LEN);

    NVIC_SetPriority(EXTI9_5_IRQn, 0);
    NVIC_EnableIRQ(EXTI9_5_IRQn);

    timerInit(TIM_COMP, 7199, 65535);   // 10k counts/sec, reset after 6.5 seconds

    swTimInit();
    rtcInit();
    rtcSetCb(compSecondsCb);

    swTimSet(SW_TIM_WHEEL_ANTIBOUNCE, SW_TIM_ON);
    swTimSet(SW_TIM_PEDAL_ANTIBOUNCE, SW_TIM_ON);
}

Comp *compGet()
{
    return &comp;
}

static void compActionGet(void)
{
    if (ACTION_NONE == action.type) {
        actionGetButtons();
    }

    if (ACTION_NONE == action.type) {
        actionGetTimers();
    }
}

static void compActionHandle(void)
{
    switch (action.type) {
    case ACTION_INIT_RTC:
        rtcInit();
        break;
    }
}

static void compScreenShow(void)
{
    canvasShowMain(true);
}

void compRun()
{
    while (1) {
        compActionGet();
        compActionHandle();
        compScreenShow();
    }
}

int32_t compGetSpeedMph(void)
{
    /*
     * Counter freq = 10000 counts / 1 sec = wCntLastTurn / time(sec)
     * So, time(sec) = wCntLastTurn / 10000;
     * Speed(mm/sec) = wLenMm / time(sec) = wLenMm * 10000 / wCntLastTurn
     * Speed(m/h) = Speed(mm/sec) * 3600(sec/h) / 1000(mm/m), so
     * Speed(m/h) = (wLenMm * 10000 / wCntLastTurn) * 3600 / 1000
     * Speed(m/h) = wLenMm * 36000 / wCntLastTurn
     */

    return comp.wLenMm * 36000 / priv.wCntLastTurn;
}

int32_t compGetTrackLengthM(void)
{
    /*
     * Track length (mm) = wLenMm * wTurns;
     */

    return comp.wLenMm * comp.wTurns / 1000;
}

int32_t compGetTrackTime(void)
{
    return comp.trackTime;
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
    comp.inMove = true;
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
    comp.inMove = true;
}

static void compTimCb(void)
{
    comp.wTurns = 0;
    comp.pTurns = 0;
    comp.inMove = false;
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
