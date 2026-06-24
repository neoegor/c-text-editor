#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "clipboard.h"
#include "memory.h"

void clipboard_init(Clipboard* cb) {
    cb->data = NULL;
}

void clipboard_set(Clipboard* cb, const char* data) {
    size_t len = strlen(data);

    cb->data = GROW_ARRAY(char, cb->data, len+1);

    if (!cb->data) {
        // Kaboom
        return;
    }

    memcpy(cb->data, data, len+1);
}

const char* clipboard_get(Clipboard* cb) {
    return cb->data ? cb->data : "";
}

void clipboard_free(Clipboard* cb) {
    free(cb->data);
    cb->data = NULL;
}
