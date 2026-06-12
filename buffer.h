#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

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
} Buffer;

void buffer_init(Buffer* buffer);
bool buffer_insert_at(Buffer* buffer, size_t line, size_t col, char ch);
bool buffer_delete_at(Buffer* buffer, size_t line, size_t col);
bool buffer_insert_line(Buffer* buffer, size_t line);
bool buffer_delete_line(Buffer* buffer, size_t line);
void buffer_free(Buffer* buffer);

#endif
