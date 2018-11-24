#include <stdlib.h>

#include "editor.h"
#include "erow.h"
#include "config.h"

extern struct editor_config E;

/*** editor operations ***/
void editor_insert_char(int c)
{
    if (E.cy == E.numrows) {
        editor_insert_row(E.numrows, "", 0);
    }
    editor_row_insert_char(&E.row[E.cy], E.cx, c);
    E.cx++;
}

void editor_del_char()
{
    if (E.cy == E.numrows) return;
    if (E.cx == 0 && E.cy == 0) return;

    erow *row = &E.row[E.cy];
    if (E.cx > 0) {
        editor_row_del_char(row, E.cx - 1);
        E.cx--;
    } else {
        E.cx = E.row[E.cy - 1].size;
        editor_row_append_string(&E.row[E.cy - 1], row->chars, row->size);
        editor_del_row(E.cy);
        E.cy--;
    }
}

void editor_insert_newline()
{
    if (E.cx == 0) {
        editor_insert_row(E.cy, "", 0);
    } else {
        erow *row = &E.row[E.cy];
        editor_insert_row(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
        row = &E.row[E.cy];
        row->size = E.cx;
        row->chars[row->size] = '\0';
        editor_update_row(row);
    }
    E.cy++;
    E.cx = 0;
}
