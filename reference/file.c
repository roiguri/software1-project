/**
 Implementation file for custom file handling functions. Please refer to the header file for more info.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

/* Custom implementation of getline due to its absence in ANSI C. Noting special */
int custom_getline(char **lineptr, size_t *n, FILE *stream)
{
    size_t pos = 0, new_size;
    int c;
    char *new_lineptr;
    if (*lineptr == NULL || *n == 0)
    {
        /* Allocate initial buffer */
        *n = 128;
        *lineptr = malloc(*n);
        if (*lineptr == NULL)
        {
            return -1;
        }
    }
    while ((c = fgetc(stream)) != EOF && c != '\n')
    {
        if (pos + 1 >= *n) /* We ensure that we have at least 1 more byte to be able to add the terminate character - */
        {
            /* Resize buffer if needed */
            new_size = *n * 2;
            new_lineptr = realloc(*lineptr, new_size);
            if (new_lineptr == NULL)
            {
                free(*lineptr);
                return -1;
            }
            *lineptr = new_lineptr;
            *n = new_size;
        }
        (*lineptr)[pos] = c;
        pos += 1;
    }
    if (c == EOF && pos == 0)
    {
        return -1; /* End of file reached and no characters read */
    }
    (*lineptr)[pos] = '\0';
    return pos;
}

void count_rows_cols_in_file(FILE *file, int *length, int *width)
{
    int col_count = 0, ret_length = 0, ret_width = -1;
    long read;
    char *line = NULL, *token;
    size_t len = 0;

    while ((read = custom_getline(&line, &len, file)) != -1)
    {
        ret_length++;
        if (ret_width == -1) /* We assume that ssumes all rows have the same number of column*/
        {
            token = strtok(line, ","); /* The entries are separated by commas, that is the way of counting the number of columns*/
            col_count = 0;
            while (token != NULL)
            {
                col_count++;
                token = strtok(NULL, ",");
            }

            ret_width = col_count;
        }
    }
    free(line); /* Free the memory allocated for the line buffer by custom_getline */
    *length = ret_length;
    *width = ret_width;
}