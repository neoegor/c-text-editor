#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include "common.h"
#include "value.h"
#include "input.h"

#define PROMPT_MAX_LENGTH 256

typedef struct {
    Rect rect;
    char prompt[PROMPT_MAX_LENGTH];
    size_t length;
    size_t cursor;
} CommandLine;

void commandline_init(CommandLine* cl, Rect rect);
void commandline_draw(CommandLine* cl);
bool commandline_handle_key(CommandLine* cl, Key key);
void commandline_clear(CommandLine* cl);
const char* commandline_get_prompt(CommandLine* cl);

#endif
