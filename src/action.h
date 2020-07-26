#ifndef ACTION_H
#define ACTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint8_t ActionType;
enum {
    ACTION_NONE = 0,

    ACTION_BTN_SHORT,
    ACTION_BTN_LONG,
    ACTION_ENCODER,
    ACTION_REMOTE,

    ACTION_INIT_HW,
    ACTION_INIT_RTC,
    ACTION_STANDBY,

    ACTION_DISP_EXPIRED,

    ACTION_TYPE_END
};

typedef uint8_t ScreenType;
enum {
    SCREEN_MAIN,

    SCREEN_MENU,

    SCREEN_END
};

typedef struct {
    ActionType type;
    int16_t value;
} Action;

typedef struct {
    ScreenType type;
    int16_t timeout;
} Screen;

#ifdef __cplusplus
}
#endif

#endif // ACTION_H
