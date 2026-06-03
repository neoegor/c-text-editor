#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

#define ALLOCATE(type, count) \
    (type*)reallocate(NULL, sizeof(type) * (count))

#define FREE(type, pointer) \
    reallocate(pointer, 0)

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, new_count) \
    (type*)reallocate(pointer, sizeof(type) * (new_count))

void* reallocate(void* pointer, size_t new_size);

#endif
