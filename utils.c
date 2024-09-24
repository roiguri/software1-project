#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "mat_utils.h"

int getline(char **lineptr, size_t *n, FILE *stream)
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

int *get_file_shape(FILE *file){
  int rows = 0, cols = 0;
  char *line = NULL, *token;
  size_t len = 0;
  long read;
  int first_row = 1;
  int *shape = malloc(2 * sizeof(int));
  if (shape == NULL){
    return NULL;
  }
  /* Count rows and columns */
  while ((read = getline(&line, &len, file)) != -1) {
      rows++;
      if (first_row){
        token = strtok(line, ",");
        while (token != NULL) {
          cols++;
          token = strtok(NULL, ",");
        }
        first_row = 0;
      }
  }
  free(line);
  shape[0] = rows;
  shape[1] = cols;
  return shape;
}

Matrix* file_to_matrix(char *filename) {
    FILE *file = fopen(filename, "r");
    int rows, cols, i, j;
    char *line = NULL, *token;
    size_t len = 0;
    long read;
    int *shape;
    Matrix *matrix;
    if (file == NULL){
      return NULL;
    }

    shape = get_file_shape(file);
    if (shape == NULL){
      return NULL;
    }
    rows = shape[0];
    cols = shape[1];
    free(shape);

    /* Reset file pointer */
    rewind(file);

    matrix = allocate_matrix(rows, cols);
    if (matrix == NULL) {
        fclose(file);
        return NULL; /* Error: memory allocation failed */
    }
    /* Read data into matrix */
    i = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        token = strtok(line, ",");
        j = 0;
        while (token != NULL && j < cols) {
            matrix->cords[i][j] = atof(token);
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }
    free(line);
    fclose(file);
    return matrix;
}

void error_has_occured()
{
  printf("An Error Has Occurred\n");
  exit(EXIT_FAILURE);
}

