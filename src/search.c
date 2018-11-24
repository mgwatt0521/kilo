#include <stdlib.h>
#include <string.h>

#include "search.h"
#include "config.h"
#include "terminal.h"
#include "ui.h"

extern struct editor_config E;


/*** find ***/

void editor_find_callback(char *query, int key)
{
    static int last_match = -1;
    static int direction = 1;

    static int saved_hl_line;
    static char *saved_hl = NULL;

    if (saved_hl) {
        memcpy(E.row[saved_hl_line].hl, saved_hl, E.row[saved_hl_line].rsize);
        free(saved_hl);
        saved_hl = NULL;
    }

    if (key == '\r'|| key == '\x1b') {
        last_match = -1;
        direction = 1;
        return;
    } else if (key == ARROW_RIGHT || key == ARROW_DOWN) {
        direction = 1;
    } else if (key == ARROW_LEFT || key == ARROW_UP) {
        direction = -1;
    } else {
        last_match = -1;
        direction = 1;
    }

    if (last_match == -1) direction = 1;
    int current = last_match;
    int i;
    for (i = 0; i < E.numrows; i++) {
        current += direction;
        if (current == -1) current = E.numrows - 1;
        else if (current == E.numrows) current = 0;

        erow *row = &E.row[current];
        char *match = strstr(row->render, query);
        if (match) {
            last_match = current;
            E.cy = current;
            E.cx = editor_row_rx_to_cx(row, match - row->render);
            E.row_off = E.numrows;

            saved_hl_line = current;
            saved_hl = malloc(row->rsize);
            memcpy(saved_hl, row->hl, row->rsize);
            memset(&row->hl[match - row->render], HL_MATCH, strlen(query));
            break;
        }
    }

}

void editor_find()
{
    int saved_cx = E.cx;
    int saved_cy = E.cy;
    int saved_coloff = E.col_off;
    int saved_rowoff = E.row_off;

    char *query = editor_prompt("Search: %s (Use ESC/Arrows/Enter)", editor_find_callback);
    if (query) {
        free(query);
    } else {
        E.cx = saved_cx;
        E.cy = saved_cy;
        E.col_off = saved_coloff;
        E.row_off = saved_rowoff;
    }
}
