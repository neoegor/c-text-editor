#include <ncurses.h>

#include "common.h"
#include "input.h"
#include "value.h"
#include "commandline.h"

static void commandline_move_left(CommandLine* cl) {

}

static void commandline_move_right(CommandLine* cl) {

}

static void commandline_insert(CommandLine* cl, char ch) {
    if (cl->length + 2 > PROMPT_MAX_LENGTH) return;

    cl->prompt[cl->cursor] = ch;
    cl->cursor++;
    cl->length++;
    cl->prompt[cl->length] = '\0';
}

static void commandline_backspace(CommandLine* cl) {
    if (cl->length == 0) return;

    cl->cursor--;
    cl->length--;
    cl->prompt[cl->length] = '\0';
}

void commandline_clear(CommandLine* cl) {
    cl->length = 0;
    cl->cursor = 0;
    cl->prompt[cl->length] = '\0';
}

const char* commandline_get_prompt(CommandLine* cl) {
    return cl->prompt;
}

void commandline_init(CommandLine* cl, Rect rect) {
    cl->rect = rect;
    cl->length = 0;
    cl->cursor = 0;
    cl->prompt[cl->length] = '\0';
}

void commandline_draw(CommandLine* cl) {
    move(cl->rect.y, 0);
    // TODO not full line just the rect
    clrtoeol();

    for (size_t col = 0; col < cl->length && col < cl->rect.width; col++) {
        mvaddch(
            cl->rect.y,
            cl->rect.x + col,
            cl->prompt[col]
        );
    }
}

bool commandline_handle_key(CommandLine* cl, Key key) {
    switch (key.type) {
        case IKEY_LEFT:
            commandline_move_left(cl);
            break;
        case IKEY_RIGHT:
            commandline_move_right(cl);
            break;
        case IKEY_BACKSPACE:
            commandline_backspace(cl);
            break;
        case IKEY_CHAR:
            commandline_insert(cl, (char)key.ch);
            break;
        case IKEY_ENTER:
            return true;

        default:
            break;
    }
    return false;
}
