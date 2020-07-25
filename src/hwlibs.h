#ifndef HWLIBS_H
#define HWLIBS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_cortex.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_rcc.h>
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

#ifdef STM32F1
#define IS_GPIO_HI(x)           ((x ## _Pin) & 0x00FF0000U)
#define IS_GPIO_LO(x)           ((x ## _Pin) & 0x0000FF00U)
#endif
#ifdef STM32F3
#define IS_GPIO_HI(x)           ((x ## _Pin) & 0x0000FF00U)
#define IS_GPIO_LO(x)           ((x ## _Pin) & 0x000000FFU)
#endif

#define READ_BYTE(p)            (IS_GPIO_LO(p) ? (READ(p) & 0x00FF) : (READ(p) & 0xFF00) >> 8)
#define WRITE_BYTE(p, data)     (CONCAT(p, _Port)->BSRR = (IS_GPIO_LO(p) ? (0x00FF0000U | (uint32_t)data) : (0xFF000000U | (uint32_t)(data << 8))))

// Project-specific definitions
#define USART_DBG               USART1

#define LL_I2C1_SCL_PIN         LL_GPIO_PIN_8
#define LL_I2C1_SDA_PIN         LL_GPIO_PIN_9

#define I2C_MASTER              I2C1

#define TIM_INPUT               TIM4
#define TIM_INPUT_HANDLER       TIM4_IRQHandler

#ifdef __cplusplus
}
#endif

#endif // HWLIBS_H
