#ifndef CONFIG_H
#define CONFIG_H

#include <time.h>
#include <termios.h>

#include "syntax.h"
#include "erow.h"

// Global constants
#define KILO_VERSION "0.0.1"
#define KILO_QUIT_TIMES 3

struct editor_config {
    int cx, cy;
    int rx;
    int row_off;
    int col_off;
    int screen_rows;
    int screen_cols;
    int numrows;
    erow *row;
    int dirty;
    char *filename;
    char statusmsg[80];
    time_t statusmsg_time;
    struct editor_syntax *syntax;
    struct termios orig_termios;
};

extern struct editor_config E;

#endif
