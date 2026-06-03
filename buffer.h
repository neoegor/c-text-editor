#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

typedef struct {
    char* chars;
    int count;
    int capacity;
} Buffer;

void buffer_init(Buffer* buffer);
void buffer_insert_ch(Buffer* buffer, char ch);
void buffer_delete_ch(Buffer* buffer);
void buffer_free(Buffer* buffer);
void buffer_print(Buffer* buffer);

#endif
