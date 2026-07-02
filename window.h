#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"
#include "input.h"
#include "buffer.h"
#include "value.h"
#include "editor.h"
#include "clipboard.h"

typedef struct {
    Rect rect;
    Rect textarea;
    Editor editor;
    bool show_line_numbers;
    int line_number_width;
    int scroll_line;
} Window;

void window_layout(Window* window);
void window_init(Window* window, Rect rect, Buffer* buffer, Clipboard* cb);
void window_draw(Window* window);
void window_handle_key(Window* window, Key key);
void window_free(Window* window);

#endif
