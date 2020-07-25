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
    uint32_t wCntCurrTurn;          // Wheel counter in the current turn
    uint32_t pCntCurrTurn;          // Pedal counter in the current turn

    uint32_t wCntLastTurn;          // Wheel counter in the last full turn
    uint32_t pCntLastTurn;          // Pedal counter in the last full turn
} CompPriv;

typedef struct {
    CompPriv *priv;

    int32_t wLenMm;                 // Length of wheel, mm

    int32_t wTurns;                 // Number of wheel turns in the trip
    int32_t pTurns;                 // Number of pedal turns in the trip
} Comp;

void compInit(void);
Comp *compGet();

void compRun(void);

uint32_t compGetSpeedMph(void);    // Get speed in m/hour

#ifdef __cplusplus
}
#endif

#endif // COMP_H
