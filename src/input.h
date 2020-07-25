#ifndef INPUT_H
#define INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define BTN_NO                  0x0000
#define ENC_NO                  BTN_NO

#define BTN_1                   0x0002
#define BTN_2                   0x0004
#define BTN_3                   0x0008

#define INPUT_BTNS              (BTN_1 | BTN_2 | BTN_3)

// Handling long press actions
#define SHORT_PRESS             60
#define LONG_PRESS              600
#define AUTOREPEAT              150

#define BTN_FLAG_NO             0x0000
#define BTN_FLAG_LONG_PRESS     0x0001

#define IN_BTN_1_Port           GPIOB
#define IN_BTN_1_Pin            LL_GPIO_PIN_3
#define IN_BTN_2_Port           GPIOB
#define IN_BTN_2_Pin            LL_GPIO_PIN_4
#define IN_BTN_3_Port           GPIOB
#define IN_BTN_3_Pin            LL_GPIO_PIN_5

typedef struct {
    uint16_t btn;
    uint16_t flags;
} CmdBtn;

typedef struct {
    uint16_t btn;
    uint16_t flags;
} Input;

void inputInit(void);
Input *inputGet(void);

CmdBtn inputGetBtnCmd(void);

#ifdef __cplusplus
}
#endif

#endif // INPUT_H
