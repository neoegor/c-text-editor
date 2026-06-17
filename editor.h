#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"
#include "buffer.h"
#include "value.h"

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
} Editor;

typedef enum {
    EKEY_UP,
    EKEY_DOWN,
    EKEY_LEFT,
    EKEY_RIGHT,
    EKEY_CHAR,
    EKEY_ENTER,
    EKEY_BACKSPACE,
    EKEY_ESCAPE,
} EditorKeyType;

typedef struct {
    EditorKeyType type;
    int ch;
} EditorKey;

typedef enum {
    EDITOR_NONE,
    EDITOR_MOVE_UP,
    EDITOR_MOVE_DOWN,
    EDITOR_MOVE_LEFT,
    EDITOR_MOVE_RIGHT,
    EDITOR_INSERT_CHAR,
    EDITOR_ENTER,
    EDITOR_BACKSPACE,
} EditorAction;

typedef struct {
    EditorAction type;
    int ch;
} EditorCommand;

void editor_init(Editor* editor, Buffer* buffer);
int editor_get_cursor_line(Editor* editor);
int editor_get_cursor_col(Editor* editor);
Mode editor_get_mode(Editor* editor);
void editor_handle_key(Editor* editor, EditorKey key);
void editor_free(Editor* editor);

#endif
