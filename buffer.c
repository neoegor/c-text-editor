#include <stdio.h>
#include <string.h>

#include "common.h"
#include "buffer.h"
#include "memory.h"

static void gb_init(GapBuffer* gb) {
    gb->chars = ALLOCATE(char, GAP_SIZE);
    gb->gap_index = 0;
    gb->gap_length = GAP_SIZE;
    gb->capacity = GAP_SIZE;
}

static void gb_print(GapBuffer* gb) {
    printf("[");
    for (size_t i = 0; i < gb->capacity; i++) {
        if (i >= gb->gap_index && i < gb->gap_index+gb->gap_length) {
            printf("_");
        } else {
            printf("%c", gb->chars[i]);
        }
    }
    printf("]\n");
}

static void gb_move_to(GapBuffer* gb, size_t col) {
    if (gb->gap_index == col) return;

    if (gb->gap_index > col) {
        while (gb->gap_index > col) {
            gb->chars[gb->gap_index+gb->gap_length-1] = gb->chars[gb->gap_index-1];
            gb->gap_index--;
        }
    } else {
        while (gb->gap_index < col) {
            gb->chars[gb->gap_index] = gb->chars[gb->gap_index+gb->gap_length];
            gb->gap_index++;
        }
    }
}

static void gb_insert_at(GapBuffer* gb, size_t col, char ch) {
    if (col > gb->capacity-gb->gap_length) return;

    if (gb->gap_length == 0) {
        gb->capacity += GAP_SIZE; 
        gb->gap_length = GAP_SIZE;
        gb->chars = GROW_ARRAY(char, gb->chars, gb->capacity);
        gb->gap_index = gb->capacity-gb->gap_length;
    }

    gb_move_to(gb, col);

    gb->chars[gb->gap_index] = ch;
    gb->gap_index++;
    gb->gap_length--;
}

static void gb_delete_at(GapBuffer* gb, size_t col) {
    if (col >= gb->capacity-gb->gap_length) return;

    gb_move_to(gb, col);

    gb->gap_length++;
}

static void gb_free(GapBuffer* gb) {
    FREE(char, gb->chars);
}

void buffer_init(Buffer* buffer) {
    buffer->capacity = 0;
    buffer->capacity = GROW_CAPACITY(buffer->capacity);
    buffer->lines = ALLOCATE(GapBuffer, buffer->capacity);
    buffer->count = 1;
    gb_init(buffer->lines);
}

void buffer_insert_at(Buffer* buffer, size_t line, size_t col, char ch) {
    if (line >= buffer->count) return;

    gb_insert_at(&buffer->lines[line], col, ch);
}

void buffer_delete_at(Buffer* buffer, size_t line, size_t col) {
    if (line >= buffer->count) return;

    gb_delete_at(&buffer->lines[line], col);
}

void buffer_free(Buffer* buffer) {
    for (size_t i = 0; i < buffer->count; i++) {
        gb_free(&buffer->lines[i]);
    }
    FREE(GapBuffer, buffer->lines);
    buffer->lines = NULL;
    buffer->count = 0;
    buffer->capacity = 0;
}
