#include <ncurses.h>
#include <string.h>

#include "common.h"
#include "window.h"
#include "value.h"
#include "clipboard.h"
#include "color.h"

void window_layout(Window* window) {
    window->textarea.height = window->rect.height - 1;
    size_t max_line_number = window->scroll_line + window->textarea.height;
    window->line_number_width = 1;

    while (max_line_number >= 10) {
        max_line_number /= 10;
        window->line_number_width++;
    }
    window->textarea.x = window->rect.x + 2 + window->line_number_width;
    window->textarea.y = window->rect.y;
    window->textarea.width = window->rect.width - window->line_number_width - 2;
}

void window_init(Window* window, Rect rect, Buffer* buffer, Clipboard* cb) {
    window->rect = rect;
    editor_init(&window->editor, buffer, cb);
    window->show_line_numbers = false;
    window->scroll_line = 0;
    window_layout(window);
}

static bool should_highlight(Editor* editor, size_t line, size_t col) {
    if (editor->mode == VISUAL_MODE) {
        Point start = editor->highlight_start;
        Point end = {editor->cursor_col, editor->cursor_line};

        if ((start.y > end.y) || (start.y == end.y && start.x > end.x)) {
            Point temp = end;
            end = start;
            start = temp;
        }

        if (line < start.y) return false;
        if (line > end.y) return false;
        if (line == start.y && line == end.y) {
            return col >= start.x && col <= end.x;
        }
        if (line == start.y) {
            return col >= start.x;
        }
        if (line == end.y) {
            return col <= end.x;
        }
        return true;
    }
    return false;
}

static void window_draw_line_numbers(Window* window) {
    for (size_t line = window->scroll_line; line < buffer_get_line_count(window->editor.buffer) && line < window->textarea.height + window->scroll_line; line++) {
        attron(COLOR_PAIR(CP_DEFAULT));
        mvprintw(
            window->rect.y + line - window->scroll_line,
            window->rect.x,
            " %*d ",
            window->line_number_width,
            line+1
        );
        attroff(COLOR_PAIR(CP_DEFAULT));
    }
}

static void window_draw_line(Window* window, size_t line) {
    size_t line_length = buffer_get_line_length(window->editor.buffer, line);

    for (size_t col = 0; col < line_length && col < window->textarea.width; col++) {
        bool sh = should_highlight(&window->editor, line, col);
        if (sh) {
            attron(COLOR_PAIR(CP_SELECT));
        } else {
            attron(COLOR_PAIR(CP_DEFAULT));
        }
        mvaddch(
            window->textarea.y + line - window->scroll_line,
            window->textarea.x + col,
            buffer_get_char_at(window->editor.buffer, line, col)
        );
        if (sh) {
            attroff(COLOR_PAIR(CP_SELECT));
        } else {
            attroff(COLOR_PAIR(CP_DEFAULT));
        }
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

    const char* filename = editor_get_filename(&window->editor);
    if (filename == NULL) {
        filename = "No Name";
    }

    attron(COLOR_PAIR(CP_STATUS_BAR_B));
    mvhline(window->rect.y + window->rect.height-1, window->rect.x, ' ', window->rect.width);
    attroff(COLOR_PAIR(CP_STATUS_BAR_B));

    attron(COLOR_PAIR(CP_STATUS_BAR_A));
    mvprintw(
        window->rect.y + window->rect.height-1,
        window->rect.x,
        " %s ",
        mode_str
    );
    attroff(COLOR_PAIR(CP_STATUS_BAR_A));

    attron(COLOR_PAIR(CP_STATUS_BAR_B));
    mvprintw(
        window->rect.y + window->rect.height-1,
        window->rect.x + strlen(mode_str) + 2, // spaces
        " %s ",
        filename
    );
    attroff(COLOR_PAIR(CP_STATUS_BAR_B));

    attron(COLOR_PAIR(CP_STATUS_BAR_C));
    char right[32];
    snprintf(right, sizeof(right), " %d:%d ", window->editor.cursor_line + 1, window->editor.cursor_col + 1);

    int right_len = strlen(right);
    int x = window->rect.x + window->rect.width - right_len;

    mvprintw(
        window->rect.y + window->rect.height-1,
        x,
        "%s",
        right
    );
    attroff(COLOR_PAIR(CP_STATUS_BAR_C));
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
    window_draw_line_numbers(window);
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
