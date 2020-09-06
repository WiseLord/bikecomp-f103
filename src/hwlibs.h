#ifndef HWLIBS_H
#define HWLIBS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_cortex.h>
#include <stm32f1xx_ll_exti.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_pwr.h>
#include <stm32f1xx_ll_rcc.h>
#include <stm32f1xx_ll_rtc.h>
#include <stm32f1xx_ll_spi.h>
#include <stm32f1xx_ll_system.h>
#include <stm32f1xx_ll_tim.h>
#include <stm32f1xx_ll_utils.h>

// Pins helper definitions
#define CONCAT(x,y)             x ## y

#define SET(p)                  (LL_GPIO_SetOutputPin(CONCAT(p, _Port), CONCAT(p, _Pin)))
#define CLR(p)                  (LL_GPIO_ResetOutputPin(CONCAT(p, _Port), CONCAT(p, _Pin)))
#define TOG(p)                  (LL_GPIO_TogglePin(CONCAT(p, _Port), CONCAT(p, _Pin)))
#ifdef STM32F1
#define READ(p)                 (LL_GPIO_ReadInputPort(CONCAT(p, _Port)) & (CONCAT(p, _Pin) >> GPIO_PIN_MASK_POS) & 0x0000FFFFU)
#else
#define READ(p)                 (LL_GPIO_ReadInputPort(CONCAT(p, _Port)) & (CONCAT(p, _Pin)) & 0x0000FFFFU)
#endif

#define OUT(p)                  (LL_GPIO_SetPinMode(CONCAT(p, _Port), CONCAT(p, _Pin), LL_GPIO_MODE_OUTPUT))
#define IN(p)                   (LL_GPIO_SetPinMode(CONCAT(p, _Port), CONCAT(p, _Pin), LL_GPIO_MODE_INPUT))

// Project-specific definitions
#define USART_DBG               USART1

#define LL_I2C1_SCL_PIN         LL_GPIO_PIN_8
#define LL_I2C1_SDA_PIN         LL_GPIO_PIN_9

#define I2C_MASTER              I2C1

#define TIM_COMP                TIM3
#define TIM_COMP_HANDLER        TIM3_IRQHandler
#define TIM_INPUT               TIM4
#define TIM_INPUT_HANDLER       TIM4_IRQHandler

#define EXTI_COMP_HANDLER       EXTI9_5_IRQHandler

#ifdef __cplusplus
}
#endif

#endif // HWLIBS_H
