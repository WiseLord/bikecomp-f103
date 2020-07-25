#ifndef COMP_H
#define COMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define WHEEL_Port              GPIOB
#define WHEEL_Pin               LL_GPIO_PIN_8
#define WHEEL_ExtiLine          LL_EXTI_LINE_8
#define WHEEL_AR_ExtiPort       LL_GPIO_AF_EXTI_PORTB
#define WHEEL_AR_ExtiLine       LL_GPIO_AF_EXTI_LINE8

#define PEDAL_Port              GPIOB
#define PEDAL_Pin               LL_GPIO_PIN_9
#define PEDAL_ExtiLine          LL_EXTI_LINE_9
#define PEDAL_AR_ExtiPort       LL_GPIO_AF_EXTI_PORTB
#define PEDAL_AR_ExtiLine       LL_GPIO_AF_EXTI_LINE9

typedef struct {
    int32_t wLenMm;     // Length of wheel, mm
    int32_t wTurns;     // Number of wheel turns

    int32_t pTurns;     // Number of pedal turns
} Comp;

void compInit(void);
Comp *compGet();

void compRun(void);

#ifdef __cplusplus
}
#endif

#endif // COMP_H
