#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include "common.h"

typedef struct {
    char* data;
} Clipboard;

void clipboard_init(Clipboard* cb);
const char* clipboard_get(Clipboard* cb);
void clipboard_set(Clipboard* cb, const char* data);
void clipboard_free(Clipboard* cb);

#endif
