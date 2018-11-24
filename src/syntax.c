#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "syntax.h"
#include "config.h"

char *C_HL_extensions[] = {".c", ".h", ".cpp", NULL};
char *C_HL_keywords[] = {
    "switch", "if", "while", "for", "break", "continue", "return", "else",
    "struct", "union", "typedef", "static", "enum", "class", "case",

    "int|", "long|", "double|", "float|", "char|", "unsigned|", "void|",
    NULL
};

struct editor_syntax HLDB[] = {
    {
        "c",
        C_HL_extensions,
        C_HL_keywords,
        "//", "/*", "*/",
        HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS | HL_HAS_MACROS
    },
};

size_t hldb_entries = (sizeof(HLDB) / sizeof(HLDB[0]));

extern struct editor_config E;

void editor_update_syntax(erow *row)
{
    row->hl = realloc(row->hl, row->rsize);
    memset(row->hl, HL_NORMAL, row->rsize);

    if (E.syntax == NULL) return;

    char **keywords = E.syntax->keywords;

    char *scs = E.syntax->singleline_comment_start;
    char *mcs = E.syntax->multiline_comment_start;
    char *mce = E.syntax->multiline_comment_end;

    int scs_len = scs ? strlen(scs) : 0;
    int mcs_len = mcs ? strlen(mcs) : 0;
    int mce_len = mce ? strlen(mcs) : 0;

    int prev_sep = 1;
    int in_string = 0;
    int in_comment = (row->idx > 0 && E.row[row->idx - 1].hl_open_comment);
    int in_macro = 0;
    int in_macro_id = 0;
    int in_macro_def = 0;

    int i = 0;
    while (i < row->rsize) {
        char c = row->render[i];
        unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;

        if (scs_len && !in_string && !in_comment) {
            if (!strncmp(&row->render[i], scs, scs_len)) {
                memset(&row->hl[i], HL_COMMENT, row->rsize - i);
                break;
            }
        }

        if (mcs_len && mce_len && !in_string) {
            if (in_comment) {
                row->hl[i] = HL_MLCOMMENT;
                if (!strncmp(&row->render[i], mce, mce_len)) {
                    memset(&row->hl[i], HL_MLCOMMENT, mce_len);
                    i += mce_len;
                    in_comment = 0;
                    prev_sep = 1;
                    continue;
                } else {
                    i++;
                    continue;
                }
            } else if (!strncmp(&row->render[i], mcs, mcs_len)) {
                memset(&row->hl[i], HL_MLCOMMENT, mcs_len);
                i += mcs_len;
                in_comment = 1;
                continue;
            }
        }

        if (E.syntax->flags & HL_HIGHLIGHT_STRINGS) {
            if (in_string) {
                row->hl[i] = HL_STRING;
                if (c == '\\' && i + 1 < row->rsize) {
                    row->hl[i + 1] = HL_STRING;
                    i += 2;
                    continue;
                }

                if (c == in_string) in_string = 0;
                i++;
                prev_sep = 1;
                continue;
            } else {
                if (c == '"' || c == '\'') {
                    in_string = c;
                    row->hl[i] = HL_STRING;
                    i++;
                    continue;
                }
            }
        }

        if (E.syntax->flags & HL_HIGHLIGHT_NUMBERS) {
            if ((isdigit(c) && (prev_sep || prev_hl == HL_NUMBER))
                || (c == '.' && prev_hl == HL_NUMBER)) {
                row->hl[i] = HL_NUMBER;
                i++;
                prev_sep = 0;
                continue;
            }
        }

        if (E.syntax->flags & HL_HAS_MACROS) {
            if (in_macro) {
                row->hl[i] = in_macro_id ? HL_MACRO_ID : HL_MACRO;

                if (in_macro_id) {
                    row->hl[i] = HL_MACRO_ID;

                    if (in_macro_def) {
                        row->hl[i] = HL_NORMAL;
                        in_macro = 0;
                        in_macro_id = 0;
                        in_macro_def = 0;
                    }

                    if (c == ' ') {
                        in_macro_def = c;
                        row->hl[i] = HL_NORMAL;
                    }
                    i++;
                    continue;
                } else {
                    if (c == ' ') {
                        in_macro_id = c;
                        row->hl[i] = HL_MACRO_ID;
                        i++;
                        continue;
                    }
                }

                if (c == ' ') {
                    in_macro_id = c;
                }
                i++;
                continue;
            } else {
                if (c == '#') {
                    in_macro = c;
                    row->hl[i] = HL_MACRO;
                    i++;
                    continue;
                }
            }
        }

        if (prev_sep) {
            int j;
            for (j = 0; keywords[j]; j++) {
                int klen = strlen(keywords[j]);
                int kw2 = keywords[j][klen - 1] == '|';
                if (kw2) klen--;

                if (!strncmp(&row->render[i], keywords[j], klen) &&
                    is_separator(row->render[i + klen])) {
                    memset(&row->hl[i], kw2 ? HL_KEYWORD2 : HL_KEYWORD1, klen);
                    i += klen;
                    break;
                }
            }
            if (keywords[j] != NULL) {
                prev_sep = 0;
                continue;
            }
        }

        prev_sep = is_separator(c);
        i++;
    }

    int changed = (row->hl_open_comment != in_comment);
    row->hl_open_comment = in_comment;
    if (changed && row->idx + 1 < E.numrows) {
        editor_update_syntax(&E.row[row->idx + 1]);
    }
}

int editor_syntax_to_color(int hl)
{
    switch (hl) {
        case HL_COMMENT:
        case HL_MLCOMMENT:
            return 36;
        case HL_KEYWORD1:
        case HL_MACRO:
            return 33;
        case HL_KEYWORD2:
            return 32;
        case HL_STRING:
            return 35;
        case HL_NUMBER:
        case HL_MACRO_ID:
            return 31;
        case HL_MATCH:
            return 34;
        default:
            return 37;
    }
}

int is_separator(int c)
{
    return isspace(c) || c == '\0' || strchr(",.()+-/*=~%<>[];", c) != NULL;
}

void editor_select_syntax_highlight()
{
    E.syntax = NULL;

    char *ext = strrchr(E.filename, '.');

    for (unsigned int j = 0; j < hldb_entries; j++) {
        struct editor_syntax *s = &HLDB[j];
        unsigned int i = 0;
        while (s->filematch[i]) {
            int is_ext = (s->filematch[i][0] == '.');
            if ((is_ext && ext && !strcmp(ext, s->filematch[i])) ||
                (!is_ext && strstr(E.filename, s->filematch[i]))) {
                    E.syntax = s;

                    int filerow;
                    for (filerow = 0; filerow < E.numrows; filerow++) {
                        editor_update_syntax(&E.row[filerow]);
                    }

                    return;
            }
            i++;
        }
    }
}
