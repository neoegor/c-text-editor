#ifndef COLOR_H
#define COLOR_H

#include "common.h"

typedef enum {
    CP_DEFAULT = 1,
    CP_SELECT,
    CP_STATUS_BAR_A,
    CP_STATUS_BAR_B,
    CP_STATUS_BAR_C,
} ColorPair;

void color_pair_init();

#endif
