#include <ncurses.h>

#include "common.h"
#include "app.h"
#include "buffer.h"

void app_init(App* app) {
    app->running = true;
    buffer_init(&app->buffer);
}

static void app_draw(App* app) {
    clear();

    printw(app->buffer.chars);

    refresh();
}

static void app_handle_key(App* app, int key) {
    if (key == 27) {
        app->running = false;
    } else if (key == KEY_BACKSPACE || key == 127) {
        buffer_delete_ch(&app->buffer);
    } else {
        buffer_insert_ch(&app->buffer, key);
    }
}

void app_run(App* app) {
    int rows, cols;
    int key;

    initscr();
    getmaxyx(stdscr, rows, cols);
    raw();
    noecho();
    keypad(stdscr, TRUE);

    while (app->running) {
        app_draw(app);

        key = getch();
        
        app_handle_key(app, key);
    }

    endwin();
}

void app_free(App* app) {
    buffer_free(&app->buffer);
}
