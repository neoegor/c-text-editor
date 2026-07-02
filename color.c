#include <ncurses.h>

#include "common.h"
#include "color.h"

void color_pair_init() {
    init_pair(CP_DEFAULT, COLOR_WHITE, -1);
    init_pair(CP_SELECT, COLOR_BLACK, 8);
    init_pair(CP_STATUS_BAR_A, COLOR_BLACK, 12);
    init_pair(CP_STATUS_BAR_B, COLOR_WHITE, 8);
    init_pair(CP_STATUS_BAR_C, COLOR_BLACK, 12);
}
