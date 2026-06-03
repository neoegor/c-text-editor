#ifndef APP_H
#define APP_H

#include "common.h"
#include "buffer.h"

typedef struct {
    bool running;
    Buffer buffer;
} App;

void app_init(App* app);
void app_run(App* app);
void app_free(App* app);

#endif
