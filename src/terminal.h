#ifndef TERMINAL_H
#define TERMINAL_H

#include "config.h"

enum editor_key {
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};

void die(const char *s);


void enable_raw_mode();


void disable_raw_mode();


int editor_read_key();


int get_window_size(int *rows, int *cols);


int get_cursor_position(int *rows, int *cols);


#endif
