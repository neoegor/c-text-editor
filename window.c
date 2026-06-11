#include <ncurses.h>

#include "common.h"
#include "window.h"
#include "value.h"

void window_init(Window* window, Rect rect, Buffer* buffer) {
    window->rect = rect;
    window->buffer = buffer;
    window->cursor_line = 0;
    window->cursor_col = 0;
}

void window_draw(Window* window) {
    for (size_t line = 0; line < window->buffer->count; line++) {
        // for (size_t col = 0; col < gb_length(&window->buffer->lines[line]); col++) {
        for (size_t col = 0; col < window->buffer->lines[line].capacity; col++) {
            GapBuffer* gb = &window->buffer->lines[line];
            if (col >= gb->gap_index && col < gb->gap_index+gb->gap_length) {
                // mvaddch(line, col, '_');
                continue;
            } else if (col < gb->gap_index) {
                mvaddch(line, col, gb->chars[col]);
            } else {
                mvaddch(line, col-gb->gap_length, gb->chars[col]);
            }
        }
    }
    move(window->cursor_line, window->cursor_col);
}

static void window_move_up(Window* window) {
    if (window->cursor_line <= 0) return;
    window->cursor_line--;
}

static void window_move_down(Window* window) {
    if (window->cursor_line >= window->rect.height - 1) return;
    window->cursor_line++;
}

static void window_move_left(Window* window) {
    if (window->cursor_col <= 0) return;
    window->cursor_col--;
}

static void window_move_right(Window* window) {
    if (window->cursor_col >= window->rect.width - 1) return;
    window->cursor_col++;
}

void window_handle_key(Window* window, int key) {
    if (key == KEY_BACKSPACE || key == 127) {
        window_move_left(window);
        buffer_delete_at(window->buffer, 0, window->cursor_col);
    } else if (key == KEY_UP) {
        window_move_up(window);
    } else if (key == KEY_DOWN) {
        window_move_down(window);
    } else if (key == KEY_LEFT) {
        window_move_left(window);
    } else if (key == KEY_RIGHT) {
        window_move_right(window);
    } else {
        buffer_insert_at(window->buffer, 0, window->cursor_col, key);
        window_move_right(window);
    }
}

void window_free(Window* window) {

}
