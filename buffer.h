#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"
#include "value.h"

#define GAP_SIZE 32

typedef struct {
    char* chars;
    size_t gap_index;
    size_t gap_length;
    size_t capacity;
} GapBuffer;

typedef struct {
    GapBuffer* lines;
    size_t count;
    size_t capacity;
    char* filename;
} Buffer;

void buffer_init(Buffer* buffer);
size_t buffer_get_line_count(Buffer* buffer);
size_t buffer_get_line_length(Buffer* buffer, size_t line);
char buffer_get_char_at(Buffer* buffer, size_t line, size_t col);
bool buffer_insert_at(Buffer* buffer, size_t line, size_t col, char ch);
bool buffer_delete_at(Buffer* buffer, size_t line, size_t col);
bool buffer_insert_empty_line(Buffer* buffer, size_t line);
bool buffer_delete_line(Buffer* buffer, size_t line);
bool buffer_split_line(Buffer* buffer, size_t line, size_t col);
bool buffer_join_with_next_line(Buffer* buffer, size_t line);
bool buffer_delete_range(Buffer* buffer, size_t line, size_t start_col, size_t end_col);
void buffer_append_line_from_cstr(Buffer* buffer, const char* line);
char* buffer_range_to_cstr(Buffer* buffer, Point start, Point end);
// void buffer_insert_cstr(Buffer* buffer, size_t line, size_t col);
void buffer_free(Buffer* buffer);

#endif
