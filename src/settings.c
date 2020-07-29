#include "settings.h"

#include "comp.h"

static const uint16_t eeMap[] = {
    [PARAM_NULL]            =   0x00,

//    [PARAM_SYSTEM_LANG]     =   {0x70,  LANG_DEFAULT},
    [PARAM_SYSTEM_RTC_CORR] =   0x73,

    [PARAM_BIKE_WHEEL_LEN]  =   0x7A,
    [PARAM_BIKE_PAR1]       =   0x7B,
    [PARAM_BIKE_PAR2]       =   0x7C,

    [PARAM_BIKE_DIST_LOW]   =   0x7E,
    [PARAM_BIKE_DIST_HIGH]  =   0x7F,
};

void settingsInit(void)
{
    eeInit(eeMap, PARAM_END);
}

int16_t settingsRead(Param param, int16_t defVal)
{
    if (param == PARAM_NULL || param >= PARAM_END) {
        return (int16_t)EE_NOT_FOUND;
    }

    uint16_t value = eeReadRaw(eeMap[param]);

    if (value != EE_NOT_FOUND) {
        return (int16_t)value;
    }

    // Return default value if not found
    return defVal;
}

void settingsStore(Param param, int16_t value)
{
    if (param == PARAM_NULL || param >= PARAM_END) {
        return;
    }

    eeUpdateRaw(eeMap[param], (uint16_t)value);
}

const uint16_t *eeMapGet(void)
{
    return eeMap;
}
