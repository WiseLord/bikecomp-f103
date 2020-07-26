#ifndef SWTIMERS_H
#define SWTIMERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SW_TIM_OFF      -1
#define SW_TIM_ON       0

typedef uint8_t SwTimer;
enum {
    // Decreasing timers
    SW_TIM_DISPLAY = 0,
    SW_TIM_WHEEL_ANTIBOUNCE,
    SW_TIM_PEDAL_ANTIBOUNCE,

    SW_TIM_RTC_INIT,

    SW_TIM_DEC_END,

    // Increasing timers
    SW_TIM_SYSTEM,

    SW_TIM_END,
};

void swTimInit(void);

void swTimSet(SwTimer timer, int32_t value);
int32_t swTimGet(SwTimer timer);

#ifdef __cplusplus
}
#endif

#endif // SWTIMERS_H
