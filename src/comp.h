#ifndef COMP_H
#define COMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "action.h"

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

typedef int16_t BikePar;
enum {
    BIKEPAR_SPEED = 0,

    BIKEPAR_TRACK,
    BIKEPAR_TRACK_TIME,
    BIKEPAR_SPEED_AVG,
    BIKEPAR_CADENCE,
    BIKEPAR_DISTANCE,

    BIKEPAR_END,
};

typedef struct {
    int32_t wLenMm;             // Length of wheel, mm

    union {
        uint32_t distanceM;      // Total distance, m
        struct {
            int16_t distLow;
            int16_t distHigh;
        };
    };

    int32_t wTurns;             // Number of wheel turns in the trip
    int32_t pTurns;             // Number of pedal turns in the trip

    uint32_t wCntCurrTurn;      // Wheel counter in the current turn
    uint32_t pCntCurrTurn;      // Pedal counter in the current turn

    uint32_t wCntLastTurn;      // Wheel counter in the last full turn
    uint32_t pCntLastTurn;      // Pedal counter in the last full turn

    BikePar par1;
    BikePar par2;
    ScreenType screen;
    bool inMove;

    int32_t trackTime;          // Track time when moving, sec
} Comp;

void compInit(void);
Comp *compGet();

void compRun(void);

int32_t compGetSpeedMph(void);          // Get speed in m/hour
int32_t compGetTrackLengthM(void);      // Get track length in meters
int32_t compGetTrackTime(void);         // Get track time in seconds
int32_t compGetAvgSpeedMph(void);       // Get average speed in m/hour
int32_t compGetCadenceRP10M(void);      // Get cadence in rounds per 10 minutes
int32_t compGetTotalDistanceM(void);    // Get total distance in m

#ifdef __cplusplus
}
#endif

#endif // COMP_H
