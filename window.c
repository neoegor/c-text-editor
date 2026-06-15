#include <ncurses.h>

#include "common.h"
#include "window.h"
#include "value.h"

const char* message = "";

void window_init(Window* window, Rect rect, Buffer* buffer) {
    window->rect = rect;
    window->buffer = buffer;
    window->cursor_line = 0;
    window->cursor_col = 0;
    window->target_col = 0;
    window->scroll_line = 0;
}

static void window_draw_gapbuffer(Window* window, size_t line) {
    GapBuffer* gb = &window->buffer->lines[line];

    for (size_t col = 0; col < gb->gap_index && col < window->rect.width; col++) {
        mvaddch(
            window->rect.y + line - window->scroll_line,
            window->rect.x + col,
            gb->chars[col]
        );
    }

    for (size_t col = gb->gap_index + gb->gap_length; col < gb->capacity && col < window->rect.width; col++) {
        mvaddch(
            window->rect.y + line - window->scroll_line,
            window->rect.x + col-gb->gap_length,
            gb->chars[col]
        );
    }
}

static void window_draw_buffer(Window* window) {
    for (size_t line = window->scroll_line; line < window->buffer->count && line < window->rect.height + window->scroll_line; line++) {
        window_draw_gapbuffer(window, line);
    }
}

static void window_draw_statusbar(Window* window) {
    mvprintw(window->rect.height-1, 0, "[ %d:%d ] [ %d ]", window->cursor_line+1, window->cursor_col+1, window->scroll_line);
}

static void window_move_cursor(Window* window) {
    move(window->cursor_line - window->scroll_line, window->cursor_col);
}

void window_draw(Window* window) {
    window_draw_buffer(window);
    window_draw_statusbar(window);
    window_move_cursor(window);
}

static void window_adjust_cursor_col(Window* window) {
    // Function :|
    size_t line_length = window->buffer->lines[window->cursor_line].capacity - window->buffer->lines[window->cursor_line].gap_length;
    if (window->target_col > line_length) {
        window->cursor_col = line_length;
    } else {
        window->cursor_col = window->target_col;
    }
}

static void window_move_up(Window* window) {
    if (window->cursor_line <= 0) return;
    window->cursor_line--;

    window_adjust_cursor_col(window);
}

static void window_move_down(Window* window) {
    if (window->cursor_line >= window->buffer->count - 1) return;
    window->cursor_line++;

    window_adjust_cursor_col(window);
}

static void window_move_left(Window* window) {
    if (window->cursor_col <= 0) return;
    window->cursor_col--;
    window->target_col = window->cursor_col;
}

static void window_move_right(Window* window) {
    // TODO super bad
    if (window->cursor_col >= window->buffer->lines[window->cursor_line].capacity - window->buffer->lines[window->cursor_line].gap_length) return;
    window->cursor_col++;
    window->target_col = window->cursor_col;
}

static void window_insert_ch(Window* window, char key) {
    if (buffer_insert_at(window->buffer, window->cursor_line, window->cursor_col, key)) {
        window_move_right(window);
    }
}

static void window_delete_line(Window* window);

static void window_delete_ch(Window* window) {
    if (window->cursor_col == 0) {
        window_delete_line(window);
        return;
    }
    window_move_left(window);
    buffer_delete_at(window->buffer, window->cursor_line, window->cursor_col);
}

static void window_insert_line(Window* window) {
    if (buffer_split_line(window->buffer, window->cursor_line, window->cursor_col)) {
        window->target_col = 0;
        window->cursor_col = window->target_col;
        window->cursor_line++;
    }
}

static void window_delete_line(Window* window) {
    if (window->cursor_line == 0) return; 

    window->cursor_line--;
    window->target_col = buffer_get_line_length(window->buffer, window->cursor_line);
    window->cursor_col = window->target_col;
    buffer_join_with_next_line(window->buffer, window->cursor_line);
}

static void window_ensure_cursor_visible(Window* window) {
    if (window->cursor_line < window->scroll_line) {
        window->scroll_line = window->cursor_line;
    } else if (window->cursor_line >= window->scroll_line + window->rect.height - 1) {
        // status bar, need text area rect
        window->scroll_line = window->cursor_line - window->rect.height + 2;
    }
}

void window_handle_key(Window* window, int key) {
    switch (key) {
        case KEY_UP:
            window_move_up(window);
            break;
        case KEY_DOWN:
            window_move_down(window);
            break;
        case KEY_LEFT:
            window_move_left(window);
            break;
        case KEY_RIGHT:
            window_move_right(window);
            break;
        case KEY_BACKSPACE:
        case 127:
        case 8:
            window_delete_ch(window);
            break;
        case '\n':
        case '\r':
            window_insert_line(window);
            break;
        
        default:
            if (key >= 32 && key <= 126) {
                window_insert_ch(window, (char)key);
            }
            break;
    }

    window_ensure_cursor_visible(window);
}

void window_free(Window* window) {
    window->buffer = NULL;
}
