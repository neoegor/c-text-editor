#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"
#include "input.h"
#include "buffer.h"
#include "value.h"
#include "clipboard.h"

typedef enum {
    NORMAL_MODE,
    INSERT_MODE,
    VISUAL_MODE,
} Mode;

typedef struct {
    Buffer* buffer;
    Mode mode;
    int cursor_line;
    int cursor_col;
    int target_col;

    Point highlight_start;
    Clipboard* cb;
} Editor;

typedef enum {
    EDITOR_NONE,
    EDITOR_MOVE_UP,
    EDITOR_MOVE_DOWN,
    EDITOR_MOVE_LEFT,
    EDITOR_MOVE_RIGHT,
    EDITOR_MOVE_EOL,
    EDITOR_MOVE_BOL,
    EDITOR_INSERT_CHAR,
    EDITOR_ENTER,
    EDITOR_BACKSPACE,
    EDITOR_TAB,
    EDITOR_OPEN_LINE_BELLOW,
    EDITOR_OPEN_LINE_ABOVE,
    EDITOR_DELETE_TO_EOL,
    EDITOR_DELETE_RANGE,
    EDITOR_YANK_RANGE,
    EDITOR_PASTE,
} EditorAction;

typedef struct {
    EditorAction type;
    int ch;
} EditorCommand;

void editor_init(Editor* editor, Buffer* buffer, Clipboard* cb);
int editor_get_cursor_line(Editor* editor);
int editor_get_cursor_col(Editor* editor);
Mode editor_get_mode(Editor* editor);
const char* editor_get_filename(Editor* editor);
void editor_handle_key(Editor* editor, Key key);
void editor_free(Editor* editor);

#endif
