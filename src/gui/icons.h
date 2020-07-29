#ifndef ICONS_H
#define ICONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "fonts.h"

typedef uint8_t Icon;
enum {
    ICON_BEGIN = 0,

    ICON_ABOVE = ICON_BEGIN,
    ICON_BELOW,

    ICON_WHEEL_BEGIN = 0x10,
    ICON_WHEEL_END = ICON_WHEEL_BEGIN + 15,

    ICON_END,
};

extern const tFont bikecompicons;

#ifdef __cplusplus
}
#endif

#endif // ICONS_H
