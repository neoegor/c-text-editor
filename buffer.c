#include <stdio.h>
#include <string.h>

#include "common.h"
#include "buffer.h"
#include "memory.h"

void buffer_init(Buffer* buffer) {
    buffer->chars = ALLOCATE(char, 1);
    *buffer->chars = '\0';
    buffer->count = 0;
    buffer->capacity = 1;
} 

void buffer_insert_ch(Buffer* buffer, char ch) {
    if (buffer->count + 2 > buffer->capacity) {
        buffer->capacity = GROW_CAPACITY(buffer->capacity);
        buffer->chars = GROW_ARRAY(char, buffer->chars, buffer->capacity);
    }

    buffer->chars[buffer->count] = ch;
    buffer->count++;
    buffer->chars[buffer->count] = '\0';
}

void buffer_delete_ch(Buffer* buffer) {
    if (buffer->count <= 0) return;
    buffer->count--;
    buffer->chars[buffer->count] = '\0';
}

void buffer_insert_at(Buffer* buffer, int col, char ch) {
    if (col < 0 || col > buffer->count) return;

    if (buffer->count + 2 > buffer->capacity) {
        buffer->capacity = GROW_CAPACITY(buffer->capacity);
        buffer->chars = GROW_ARRAY(char, buffer->chars, buffer->capacity);
    }

    memmove(buffer->chars + col + 1, buffer->chars + col, buffer->count - col + 1);
    buffer->count++;
    buffer->chars[col] = ch;
}

void buffer_delete_at(Buffer* buffer, int col) {
    if (col <= 0 || col > buffer->count) return;

    memmove(buffer->chars + col - 1, buffer->chars + col, buffer->count - col + 1);
    buffer->count--;
}

void buffer_free(Buffer* buffer) {
    FREE(char, buffer->chars);
    buffer_init(buffer);
}

void buffer_print(Buffer* buffer) {
    printf("Buffer contents: \"%s\"\n", buffer->chars);
}
