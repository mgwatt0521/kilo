#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "io.h"
#include "ui.h"
#include "config.h"
#include "terminal.h"

extern struct editor_config E;

/*** file I/O ***/
void editor_open(char *filename)
{
    free(E.filename);
    E.filename = strdup(filename);

    editor_select_syntax_highlight();

    FILE *fp = fopen(filename, "r");
    if (!fp) die("open");

    char *line = NULL;
    size_t line_cap = 0;
    ssize_t line_len;
    while ((line_len = getline(&line, &line_cap, fp)) != -1) {
        while (line_len > 0 && (line[line_len - 1] == '\n' ||
                                line[line_len - 1] == '\r'))
            line_len--;
        editor_insert_row(E.numrows, line, line_len);
    }
    free(line);
    fclose(fp);
    E.dirty = 0;
}

char *editor_rows_to_string(int *buflen)
{
    int totlen = 0;
    int j;
    for (j = 0; j < E.numrows; j++) {
        totlen += E.row[j].size + 1;
    }
    *buflen = totlen;

    char *buf = malloc(totlen);
    char *p = buf;
    for (j = 0; j < E.numrows; j++) {
        memcpy(p, E.row[j].chars, E.row[j].size);
        p += E.row[j].size;
        *p = '\n';
        p++;
    }

    return buf;
}

void editor_save()
{
    if (E.filename == NULL) {
        E.filename = editor_prompt("Save as: %s", NULL);
        if (E.filename == NULL) {
            editor_set_status_message("Save aborted.");
            return;
        }
        editor_select_syntax_highlight();
    }

    int len;
    char *buf = editor_rows_to_string(&len);
    int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
    if (fd != -1) {
        if (ftruncate(fd, len) != -1) {
            if (write(fd, buf, len) == len) {
                close(fd);
                free(buf);
                E.dirty = 0;
                editor_set_status_message("%d bytes written to disk", len);
                return;
            }
        }
        close(fd);
    }

    free(buf);
    editor_set_status_message("Can't save! I/O error: %s", strerror(errno));
}
