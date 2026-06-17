#include <ncurses.h>

#include "common.h"
#include "app.h"
#include "buffer.h"
#include "value.h"
#include "window.h"

void app_init(App* app) {
    int rows, cols;
    app->running = true;
    initscr();
    getmaxyx(stdscr, rows, cols);
    raw();
    noecho();
    keypad(stdscr, TRUE);
    set_escdelay(25);

    buffer_init(&app->buffer);
    window_init(&app->window, (Rect){0, 0, cols, rows}, &app->buffer);
}

static void app_draw(App* app) {
    clear();
    window_draw(&app->window);
    refresh();
}

static void app_handle_key(App* app, int key) {
    if (key == CTRL_C) {
        app->running = false;
    } else {
        window_handle_key(&app->window, key);
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
