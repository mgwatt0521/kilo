#ifndef IO_H
#define IO_H

/*** file I/O ***/
void editor_open(char *filename);


char *editor_rows_to_string(int *buflen);


void editor_save();


#endif
