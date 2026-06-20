#ifndef APP_H
#define APP_H

#include "common.h"
#include "buffer.h"
#include "window.h"
#include "commandline.h"

typedef enum {
    APP_NORMAL,
    APP_COMMAND,
} AppMode;

typedef struct {
    bool running;
    AppMode mode;
    CommandLine cl;
    Buffer buffer;
    Window window;
} App;

void app_init(App* app, int argc, const char* argv[]);
void app_run(App* app);
void app_free(App* app);

#endif
