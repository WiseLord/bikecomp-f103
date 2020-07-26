#include "settings.h"

#include "comp.h"
#include "display/glcd.h"
#include "eemul.h"
#include "input.h"
//#include "tr/labels.h"

static int16_t rtcCorr = 0;

#define GENERATE_EE_RC_MAP(CMD)  [PARAM_RC_ ## CMD] = {0x80 + RC_CMD_ ## CMD, (int16_t)EE_NOT_FOUND},

static const EE_Cell eeMap[] = {
    [PARAM_NULL]            =   {0x00,  0},

//    [PARAM_DISPLAY_PALETTE] =   {0x44,  PAL_DEFAULT},

//    [PARAM_SYSTEM_LANG]     =   {0x70,  LANG_DEFAULT},
    [PARAM_SYSTEM_RTC_CORR] =   {0x73,  0},

    [PARAM_BIKE_WHEEL_LEN]  =   {0x7A,  2062},
    [PARAM_BIKE_PAR1]       =   {0x7B,  BIKEPAR_TRACK},
    [PARAM_BIKE_PAR2]       =   {0x7C,  BIKEPAR_TRACK_TIME},
};

void settingsInit(void)
{
    eeInit(eeMap, PARAM_END);

    for (Param par = PARAM_NULL + 1; par < PARAM_END; par++) {
        settingsSet(par, settingsRead(par));
    }
}

int16_t settingsGet(Param param)
{
    int16_t ret = 0;

    Comp *comp = compGet();

    switch (param) {

    case PARAM_DISPLAY_PALETTE:
//        ret = paletteGetIndex();
        break;
    case PARAM_SYSTEM_LANG:
//        ret = labelsGetLang();
        break;
    case PARAM_SYSTEM_RTC_CORR:
        ret = rtcCorr;
        break;

    case PARAM_BIKE_WHEEL_LEN:
        ret = comp->wLenMm;
        break;
    case PARAM_BIKE_PAR1:
        ret = comp->par1;
        break;
    case PARAM_BIKE_PAR2:
        ret = comp->par2;
        break;

    default:
        break;
    }

    return  ret;
}

void settingsSet(Param param, int16_t value)
{
    Comp *comp = compGet();

    switch (param) {

    case PARAM_DISPLAY_PALETTE:
//        paletteSetIndex((PalIdx)value);
        break;

    case PARAM_SYSTEM_LANG:
//        labelsSetLang((Lang)value);
        break;
    case PARAM_SYSTEM_RTC_CORR:
        rtcCorr = value;
        break;

    case PARAM_BIKE_WHEEL_LEN:
        comp->wLenMm = value;
        break;
    case PARAM_BIKE_PAR1:
        comp->par1 = value;
        break;
    case PARAM_BIKE_PAR2:
        comp->par2= value;
        break;

    default:
        break;
    }
}

int16_t settingsRead(Param param)
{
    if (param == PARAM_NULL || param >= PARAM_END) {
        return (int16_t)EE_NOT_FOUND;
    }

    uint16_t value = eeReadRaw(eeMap[param].cell);

    if (value != EE_NOT_FOUND) {
        return (int16_t)value;
    }

    // Return default value if not found
    return eeMap[param].value;
}

void settingsStore(Param param, int16_t value)
{
    if (param == PARAM_NULL || param >= PARAM_END) {
        return;
    }

    eeUpdateRaw(eeMap[param].cell, (uint16_t)value);
}

const EE_Cell *eeMapGet(void)
{
    return eeMap;
}
