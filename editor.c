#include "common.h"
#include "input.h"
#include "editor.h"
#include "buffer.h"

static void editor_adjust_cursor_col(Editor* editor) {
    size_t line_length = buffer_get_line_length(editor->buffer, editor->cursor_line);
    if (editor->target_col > line_length) {
        editor->cursor_col = line_length;
    } else {
        editor->cursor_col = editor->target_col;
    }
}

static void editor_move_up(Editor* editor) {
    if (editor->cursor_line == 0) return;
    editor->cursor_line--;

    editor_adjust_cursor_col(editor);
}

static void editor_move_down(Editor* editor) {
    if (editor->cursor_line >= buffer_get_line_count(editor->buffer) - 1) return;
    editor->cursor_line++;

    editor_adjust_cursor_col(editor);
}

static void editor_move_left(Editor* editor) {
    if (editor->cursor_col <= 0) return;
    editor->cursor_col--;
    editor->target_col = editor->cursor_col;
}

static void editor_move_right(Editor* editor) {
    if (editor->cursor_col >= buffer_get_line_length(editor->buffer, editor->cursor_line)) return;
    editor->cursor_col++;
    editor->target_col = editor->cursor_col;
}

static void editor_insert_ch(Editor* editor, char key) {
    if (buffer_insert_at(editor->buffer, editor->cursor_line, editor->cursor_col, key)) {
        editor_move_right(editor);
    }
}

static void editor_delete_line(Editor* editor);

static void editor_delete_ch(Editor* editor) {
    if (editor->cursor_col == 0) {
        editor_delete_line(editor);
        return;
    }
    editor_move_left(editor);
    buffer_delete_at(editor->buffer, editor->cursor_line, editor->cursor_col);
}

static void editor_insert_tab(Editor* editor) {
    for (int i = 0; i < 4; i++) {
        editor_insert_ch(editor, ' ');
    }
}

static void editor_insert_line(Editor* editor) {
    if (buffer_split_line(editor->buffer, editor->cursor_line, editor->cursor_col)) {
        editor->target_col = 0;
        editor->cursor_col = editor->target_col;
        editor->cursor_line++;
    }
}

static void editor_delete_line(Editor* editor) {
    if (editor->cursor_line == 0) return; 

    editor->cursor_line--;
    editor->target_col = buffer_get_line_length(editor->buffer, editor->cursor_line);
    editor->cursor_col = editor->target_col;
    buffer_join_with_next_line(editor->buffer, editor->cursor_line);
}

static void editor_move_to_eol(Editor* editor) {
    editor->cursor_col = buffer_get_line_length(editor->buffer, editor->cursor_line);
}

static void editor_move_to_bol(Editor* editor) {
    editor->cursor_col = 0;
}

void editor_init(Editor* editor, Buffer* buffer) {
    editor->buffer = buffer;
    editor->mode = NORMAL_MODE;
    editor->cursor_line = 0;
    editor->cursor_col = 0;
    editor->target_col = 0;
}

int editor_get_cursor_line(Editor* editor) {
    return editor->cursor_line;
}

int editor_get_cursor_col(Editor* editor) {
    return editor->cursor_col;
}

Mode editor_get_mode(Editor* editor) {
    return editor->mode;
}

const char* editor_get_filename(Editor* editor) {
    return editor->buffer->filename;
}

static void editor_handle_command(Editor* editor, EditorCommand cmd) {
    switch (cmd.type) {
        case EDITOR_NONE:
            return;
        case EDITOR_MOVE_UP:
            editor_move_up(editor);
            break;
        case EDITOR_MOVE_DOWN:
            editor_move_down(editor);
            break;
        case EDITOR_MOVE_LEFT:
            editor_move_left(editor);
            break;
        case EDITOR_MOVE_RIGHT:
            editor_move_right(editor);
            break;
        case EDITOR_INSERT_CHAR:
            editor_insert_ch(editor, (char)cmd.ch);
            break;
        case EDITOR_ENTER:
            editor_insert_line(editor);
            break;
        case EDITOR_BACKSPACE:
            editor_delete_ch(editor);
            break;
        case EDITOR_TAB:
            editor_insert_tab(editor);
            break;
        case EDITOR_OPEN_LINE_BELLOW:
            editor_move_to_eol(editor);
            editor_insert_line(editor);
            editor->mode = INSERT_MODE;
            break;
        case EDITOR_OPEN_LINE_ABOVE:
            editor_move_to_bol(editor);
            editor_insert_line(editor);
            editor_move_up(editor);
            editor->mode = INSERT_MODE;
            break;
        
        default:
            break;
    }
}

static void editor_handle_normal_mode(Editor* editor, Key key, EditorCommand* cmd) {
    switch (key.type) {
        case IKEY_CHAR:
            switch (key.ch) {
                case 'i':
                    editor->mode = INSERT_MODE;
                    break;
                case 'h':
                    *cmd = (EditorCommand){EDITOR_MOVE_LEFT, 0};
                    break;
                case 'j':
                    *cmd = (EditorCommand){EDITOR_MOVE_DOWN, 0};
                    break;
                case 'k':
                    *cmd = (EditorCommand){EDITOR_MOVE_UP, 0};
                    break;
                case 'l':
                    *cmd = (EditorCommand){EDITOR_MOVE_RIGHT, 0};
                    break;
                case 'o':
                    *cmd = (EditorCommand){EDITOR_OPEN_LINE_BELLOW, 0};
                    break;
                case 'O':
                    *cmd = (EditorCommand){EDITOR_OPEN_LINE_ABOVE, 0};
                    break;
            }
            break;

        default:
            return;
    }
}

static void editor_handle_insert_mode(Editor* editor, Key key, EditorCommand* cmd) {
    switch (key.type) {
        case IKEY_ESCAPE:
            editor->mode = NORMAL_MODE;
            break;
        case IKEY_UP:
            *cmd = (EditorCommand){EDITOR_MOVE_UP, 0};
            break;
        case IKEY_DOWN:
            *cmd = (EditorCommand){EDITOR_MOVE_DOWN, 0};
            break;
        case IKEY_LEFT:
            *cmd = (EditorCommand){EDITOR_MOVE_LEFT, 0};
            break;
        case IKEY_RIGHT:
            *cmd = (EditorCommand){EDITOR_MOVE_RIGHT, 0};
            break;
        case IKEY_BACKSPACE:
            *cmd = (EditorCommand){EDITOR_BACKSPACE, 0};
            break;
        case IKEY_ENTER:
            *cmd = (EditorCommand){EDITOR_ENTER, 0};
            break;
        case IKEY_CHAR:
            *cmd = (EditorCommand){EDITOR_INSERT_CHAR, key.ch};
            break;
        case IKEY_TAB:
            *cmd = (EditorCommand){EDITOR_TAB, 0};
            break;

        default:
            return;
    }
}

void editor_handle_key(Editor* editor, Key key) {
    EditorCommand cmd = {EDITOR_NONE, 0};

    switch (editor->mode) {
        case NORMAL_MODE:
            editor_handle_normal_mode(editor, key, &cmd);
            break;
        case INSERT_MODE:
            editor_handle_insert_mode(editor, key, &cmd);
            break;
        case VISUAL_MODE:
            break;

        default:
            return;
    }

    editor_handle_command(editor, cmd);
}

void editor_free(Editor* editor) {
    editor->buffer = NULL;
}
