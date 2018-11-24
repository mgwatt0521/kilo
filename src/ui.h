#ifndef UI_H
#define UI_H

#include "abuf.h"

/*** output ***/

void editor_refresh_screen();


void editor_draw_rows(struct abuf *ab);


void editor_scroll();


void editor_draw_status_bar(struct abuf *ab);


void editor_draw_message_bar(struct abuf *ab);


void editor_set_status_message(const char *fmt, ...);


/*** input ***/
void editor_process_keypress();


void editor_move_cursor(int key);


char *editor_prompt(char *prompt, void (*callback)(char *, int));



#endif
