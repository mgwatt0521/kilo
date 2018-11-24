#ifndef EROW_H
#define EROW_H
typedef struct erow {
    int idx;
    int size;
    int rsize;
    char *chars;
    char *render;
    unsigned char *hl;
    int hl_open_comment;
} erow;

/*** row operations ***/

void editor_update_row(erow *row);


int editor_row_cx_to_rx(erow *row, int cx);


int editor_row_rx_to_cx(erow *row, int rx);


void editor_row_insert_char(erow *row, int at, int c);


void editor_row_del_char(erow *row, int at);


void editor_free_row(erow *row);


void editor_row_append_string(erow *row, char *s, size_t len);


void editor_insert_row(int at, char *s, size_t len);


void editor_del_row(int at);


#endif
