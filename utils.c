#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "mat_utils.h"

Matrix* file_to_matrix(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL; // Error: file could not be opened
    }

    int rows, cols;
    char *line = NULL, *token;
    size_t len = 0, read;
    int shape[2] = get_file_shape(file);
    rows = shape[0];
    cols = shape[1];

    // Reset file pointer
    rewind(file);

    Matrix *matrix = allocate_matrix(rows, cols);
    if (matrix == NULL) {
        fclose(file);
        return NULL; // Error: memory allocation failed
    }
    // Read data into matrix
    int i = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        char *token = strtok(line, ",");
        int j = 0;
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

int *get_file_shape(FILE *file){
  int shape[2];
  int rows = 0, cols = 0;
  char *line = NULL, *token;
  size_t len = 0;
  size_t read;
  int first_row = 1;

  // Count rows and columns
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

void error()
{
  printf("An Error Has Occurred\n");
  exit(EXIT_FAILURE);
}

