/**
 * This file declares functions for custom file handling, including reading lines
 * and counting rows and columns in a file.
 */
#ifndef FILE_H
#define FILE_H

#include <stdio.h>
/**
 * Reads a line from a file stream.
 * This function is similar to the standard getline function but is
 * implemented as a custom version because getline is not part of ANSI C. 
 */
int custom_getline(char **lineptr, size_t *n, FILE *stream);

/**
 * Counts the number of rows and columns in a file where the entries are separated by commas.
 * Assumes all rows have the same number of columns; uses column count from the first row. 
 */
void count_rows_cols_in_file(FILE *file, int *length, int *width);

#endif /* FILE_H */