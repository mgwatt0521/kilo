#ifndef SYNTAX_H
#define SYNTAX_H

#include "erow.h"

#define HL_HIGHLIGHT_NUMBERS (1<<0)
#define HL_HIGHLIGHT_STRINGS (1<<1)

extern size_t hldb_entries;

struct editor_syntax {
    char *filetype;
    char **filematch;
    char **keywords;
    char *singleline_comment_start;
    char *multiline_comment_start;
    char *multiline_comment_end;
    int flags;
};

enum editor_highlight {
    HL_NORMAL = 0,
    HL_COMMENT,
    HL_MLCOMMENT,
    HL_KEYWORD1,
    HL_KEYWORD2,
    HL_STRING,
    HL_NUMBER,
    HL_MATCH
};

void editor_update_syntax(erow *row);


int editor_syntax_to_color(int hl);


int is_separator(int c);


void editor_select_syntax_highlight();

#endif
