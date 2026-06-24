#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "app.h"
#include "buffer.h"
#include "value.h"
#include "window.h"
#include "commandline.h"
#include "clipboard.h"

static void app_load_file(App* app, const char* filename) {
    FILE* f = fopen(filename, "r");

    if (!f) {
        // errors
        return;
    }

    // temp until multimple buffers
    buffer_free(&app->buffer);
    buffer_init(&app->buffer);
    editor_init(&app->window.editor, &app->buffer, &app->cb);

    char line[1024];
    if (app->buffer.filename != NULL) {
        free(app->buffer.filename);
    }
    app->buffer.filename = strdup(filename);

    while (fgets(line, sizeof(line), f)) {
        buffer_append_line_from_cstr(&app->buffer, line);
    }

    fclose(f);
}

static void app_save_file_as(App* app, const char* filename) {
    FILE* f = fopen(filename, "w");

    if (!f) {
        return;
    }

    char* new_name = strdup(filename);

    if (!new_name) {
        fclose(f);
        return;
    }

    if (app->buffer.filename != NULL) {
        free(app->buffer.filename);
    }

    app->buffer.filename = new_name;

    Buffer* buffer = &app->buffer;
    size_t line_count = buffer_get_line_count(buffer);

    for (size_t line = 0; line < line_count; line++) {
        size_t len = buffer_get_line_length(buffer, line);

        for (size_t col = 0; col < len; col++) {
            char c = buffer_get_char_at(buffer, line, col);
            fputc(c, f);
        }

        if (line != line_count - 1) {
            fputc('\n', f);
        }
    }

    fclose(f);
}

static void app_save_file(App* app) {
    const char* filename = editor_get_filename(&app->window.editor);

    if (filename == NULL) return;

    app_save_file_as(app, filename);
}

void app_init(App* app, int argc, const char* argv[]) {
    app->running = true;
    app->mode = APP_NORMAL;

    int rows, cols;
    initscr();
    getmaxyx(stdscr, rows, cols);
    raw();
    noecho();
    keypad(stdscr, TRUE);
    set_escdelay(25);

    clipboard_init(&app->cb);
    commandline_init(&app->cl, (Rect){0, rows-1, cols, 1});
    if (argc == 1) {
        buffer_init(&app->buffer);
    } else if (argc == 2) {
        buffer_init(&app->buffer);
        app_load_file(app, argv[1]);
    }
    window_init(&app->window, (Rect){0, 0, cols, rows}, &app->buffer, &app->cb);
}

static void app_draw(App* app) {
    clear();
    window_draw(&app->window);

    if (app->mode == APP_COMMAND) {
        commandline_draw(&app->cl);
    }
    refresh();
}

static bool translate_key(int key, Key* out) {
    switch (key) {
        case KEY_UP:
            *out = (Key){IKEY_UP, 0};
            break;
        case KEY_DOWN:
            *out = (Key){IKEY_DOWN, 0};
            break;
        case KEY_LEFT:
            *out = (Key){IKEY_LEFT, 0};
            break;
        case KEY_RIGHT:
            *out = (Key){IKEY_RIGHT, 0};
            break;
        case KEY_BACKSPACE:
        case 127:
        case 8:
            *out = (Key){IKEY_BACKSPACE, 0};
            break;
        case '\n':
        case '\r':
            *out = (Key){IKEY_ENTER, 0};
            break;
        case 27:
            *out = (Key){IKEY_ESCAPE, 0};
            break;
        case '\t':
            *out = (Key){IKEY_TAB, 0};
            break;
        case 3:
            *out = (Key){IKEY_CTRL_C, 0};
            break;
        
        default:
            if (key >= 32 && key <= 126) {
                *out = (Key){IKEY_CHAR, key};
            } else {
                return false;
            }
            break;
    }
    return true;
}

static void app_execute_command(App* app) {
    const char* cmd = commandline_get_prompt(&app->cl);

    if (strcmp(cmd, "q") == 0) {
        app->running = false;
    } else if (strncmp(cmd, "e ", 2) == 0) {
        const char* filename = cmd + 2;
        app_load_file(app, filename);
    } else if (strcmp(cmd, "w") == 0) {
        app_save_file(app);
    } else if (strncmp(cmd, "w ", 2) == 0) {
        const char* filename = cmd + 2;
        app_save_file_as(app, filename);
    }
}

static void app_handle_key(App* app, int key) {
    Key key_event;
    if (!translate_key(key, &key_event)) return;

    if (app->mode == APP_COMMAND) {
        if (key_event.type == IKEY_ESCAPE) {
            app->mode = APP_NORMAL;
            return;
        }
        if (commandline_handle_key(&app->cl, key_event)) {
            app_execute_command(app);
            commandline_clear(&app->cl);
            app->mode = APP_NORMAL;
        }
    } else {
        if (key_event.type == IKEY_CHAR && key_event.ch == ':' && editor_get_mode(&app->window.editor) == NORMAL_MODE) {
            app->mode = APP_COMMAND;
            return;
        }
        window_handle_key(&app->window, key_event);
    }
}

void app_run(App* app) {
    int key;

    while (app->running) {
        app_draw(app);

        key = getch();
        
        app_handle_key(app, key);
    }
}

void app_free(App* app) {
    endwin();
    buffer_free(&app->buffer);
    window_free(&app->window);
    clipboard_free(&app->cb);
}
