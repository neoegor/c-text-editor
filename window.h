#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"
#include "buffer.h"
#include "value.h"

typedef struct {
    Rect rect;
    Buffer* buffer;
    int cursor_line;
    int cursor_col;
} Window;

void window_init(Window* window, Rect rect, Buffer* buffer);
void window_draw(Window* window);
void window_handle_key(Window* window, int key);
void window_free(Window* window);

#endif
