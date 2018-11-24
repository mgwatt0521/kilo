/*** includes ***/
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include "config.h"
#include "syntax.h"
#include "erow.h"
#include "terminal.h"
#include "editor.h"
#include "ui.h"
#include "io.h"

/*** data ***/

extern struct editor_config E;

void init_editor();

/*** init ***/
int main(int argc, char *argv[])
{
    enable_raw_mode();
    init_editor();
    if (argc >= 2) {
        editor_open(argv[1]);
    }

    editor_set_status_message("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
    while (1) {
        editor_refresh_screen();
        editor_process_keypress();
    }

    return 0;
}

void init_editor()
{
    E.cx = 0;
    E.cy = 0;
    E.rx = 0;
    E.row_off = 0;
    E.col_off = 0;
    E.numrows = 0;
    E.row = NULL;
    E.dirty = 0;
    E.filename = NULL;
    E.statusmsg[0] = '\0';
    E.statusmsg_time = 0;
    E.syntax = NULL;

    if (get_window_size(&E.screen_rows, &E.screen_cols) == -1) die("get_window_size");
    E.screen_rows -= 2;
}
