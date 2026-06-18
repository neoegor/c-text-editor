#include <ncurses.h>
#include <string.h>

#include "common.h"
#include "app.h"
#include "buffer.h"
#include "value.h"
#include "window.h"
#include "commandline.h"

void app_init(App* app) {
    app->running = true;
    app->mode = APP_NORMAL;

    int rows, cols;
    initscr();
    getmaxyx(stdscr, rows, cols);
    raw();
    noecho();
    keypad(stdscr, TRUE);
    set_escdelay(25);

    commandline_init(&app->cl, (Rect){0, rows-1, cols, 1});
    buffer_init(&app->buffer);
    window_init(&app->window, (Rect){0, 0, cols, rows}, &app->buffer);
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

static void save_file_as(Editor* editor, const char* filename) {

}

static void app_execute_command(App* app) {
    const char* cmd = commandline_get_prompt(&app->cl);

    if (strcmp(cmd, "q") == 0) {
        app->running = false;
    } else if (strncmp(cmd, "w ", 2) == 0) {
        const char* filename = cmd + 2;
        save_file_as(&app->window.editor, filename);
    }
}

static void app_handle_key(App* app, int key) {
    Key key_event;
    if (!translate_key(key, &key_event)) return;

    if (key_event.type == IKEY_CTRL_C) {
        app->running = false;
    } else {
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
}
