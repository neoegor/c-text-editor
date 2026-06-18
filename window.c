#include <ncurses.h>

#include "common.h"
#include "window.h"
#include "value.h"

void window_init(Window* window, Rect rect, Buffer* buffer) {
    window->rect = rect;
    window->textarea = (Rect){rect.x, rect.y, rect.width, rect.height-1};
    editor_init(&window->editor, buffer);
    window->show_line_numbers = false;
    window->scroll_line = 0;
}

static void window_draw_line(Window* window, size_t line) {
    size_t line_length = buffer_get_line_length(window->editor.buffer, line);

    for (size_t col = 0; col < line_length && col < window->textarea.width; col++) {
        mvaddch(
            window->textarea.y + line - window->scroll_line,
            window->textarea.x + col,
            buffer_get_char_at(window->editor.buffer, line, col)
        );
    }
}

static void window_draw_buffer(Window* window) {
    for (size_t line = window->scroll_line; line < buffer_get_line_count(window->editor.buffer) && line < window->textarea.height + window->scroll_line; line++) {
        window_draw_line(window, line);
    }
}

static void window_draw_statusbar(Window* window) {
    int cursor_line = editor_get_cursor_line(&window->editor);
    int cursor_col = editor_get_cursor_col(&window->editor);
    Mode mode = editor_get_mode(&window->editor);
    const char* mode_str = "UNKNOWN";
    
    switch (mode) {
        case NORMAL_MODE:
            mode_str = "NORMAL";
            break;
        case INSERT_MODE:
            mode_str = "INSERT";
            break;
        case VISUAL_MODE:
            mode_str = "VISUAL";
            break;
    }

    mvprintw(
        window->rect.y + window->rect.height-1,
        window->rect.x,
        "[-- %s --] [ %d:%d ]",
        mode_str,
        cursor_line+1,
        cursor_col+1
    );
}

static void window_move_cursor(Window* window) {
    int cursor_line = editor_get_cursor_line(&window->editor);
    int cursor_col = editor_get_cursor_col(&window->editor);

    if (editor_get_mode(&window->editor) == INSERT_MODE) {
        printf("\x1b[6 q"); // beam
    } else {
        printf("\x1b[2 q"); // block
    }
    fflush(stdout);

    // Could be outside the textarea width
    move(
        window->textarea.y + cursor_line - window->scroll_line,
        window->textarea.x + cursor_col
    );
}

void window_draw(Window* window) {
    window_draw_buffer(window);
    window_draw_statusbar(window);
    window_move_cursor(window);
}

static void window_ensure_cursor_visible(Window* window) {
    int cursor_line = editor_get_cursor_line(&window->editor);

    if (cursor_line < window->scroll_line) {
        window->scroll_line = cursor_line;
    } else if (cursor_line >= window->scroll_line + window->textarea.height) {
        window->scroll_line = cursor_line - window->textarea.height + 1;
    }
}

void window_handle_key(Window* window, Key key) {
    editor_handle_key(&window->editor, key);
    window_ensure_cursor_visible(window);
}

void window_free(Window* window) {
    editor_free(&window->editor);
}
