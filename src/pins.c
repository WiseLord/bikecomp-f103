#include "pins.h"
#include <stdint.h>

static void pinsInitDisplay(void)
{
    LL_GPIO_InitTypeDef initDef;

    initDef.Mode = LL_GPIO_MODE_OUTPUT;
    initDef.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    initDef.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
#ifdef STM32F3
    initDef.Pull = LL_GPIO_PULL_NO;
#endif

    initDef.Pin = DISP_BCKL_Pin;
    LL_GPIO_Init(DISP_BCKL_Port, &initDef);

    SET(DISP_BCKL);
}

void pinsInit(void)
{
#ifdef STM32F1
    LL_GPIO_AF_Remap_SWJ_NOJTAG();
#endif

    // Enable clock for all GPIO peripherials
#ifdef STM32F1
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
#endif
#ifdef STM32F3
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
#endif

    LL_GPIO_InitTypeDef initDef;

    initDef.Mode = LL_GPIO_MODE_OUTPUT;
    initDef.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    initDef.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
#ifdef STM32F3
    initDef.Pull = LL_GPIO_PULL_NO;
#endif

    initDef.Pin = LED1_Pin;
    LL_GPIO_Init(LED1_Port, &initDef);
    initDef.Pin = LED3_Pin;
    LL_GPIO_Init(LED1_Port, &initDef);

    pinsInitDisplay();
}
