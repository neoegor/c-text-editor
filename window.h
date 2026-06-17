#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"
#include "buffer.h"
#include "value.h"
#include "editor.h"

typedef struct {
    Rect rect;
    Rect textarea;
    Editor editor;
    bool show_line_numbers;
    int scroll_line;
} Window;

void window_init(Window* window, Rect rect, Buffer* buffer);
void window_draw(Window* window);
void window_handle_key(Window* window, int key);
void window_free(Window* window);

#endif
