#ifndef INPUT_H
#define INPUT_H

#include "common.h"

typedef enum {
    IKEY_UP,
    IKEY_DOWN,
    IKEY_LEFT,
    IKEY_RIGHT,
    IKEY_CHAR,
    IKEY_ENTER,
    IKEY_BACKSPACE,
    IKEY_ESCAPE,
    IKEY_CTRL_C,
} KeyType;

typedef struct {
    KeyType type;
    int ch;
} Key;

#endif
