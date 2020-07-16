#ifndef COMP_H
#define COMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int32_t wLenMm;     // Length of wheel, mm
    int32_t wTurns;     // Number of turns
} Comp;

void compInit(void);
Comp *compGet();

void compRun(void);

#ifdef __cplusplus
}
#endif

#endif // COMP_H
