#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mat_utils.h"

/* free matrix rows from 0 to (row-1) */
void free_to_row(Matrix* X, int row){
  int i;
  for (i = 0; i < row; i++){
    free((X->cords)[i]);
  }
  free(X->cords);
  free(X);
}

void free_matrix(Matrix *A){
  if (A != NULL){
    free__to_index(A, A->rows); /* frees matrix to the last row */
  }
} 

void free_matrix2(Matrix *A, Matrix *B){
  free_matrix(A);
  free_matrix(B);
}

void free_matrix3(Matrix *A, Matrix *B, Matrix *C){
  free_matrix(A);
  free_matrix2(B, C);
}

Matrix* allocate_matrix(int rows, int cols){
  int i;
  Matrix* result = (Matrix *)malloc(sizeof(Matrix));
  if (result == NULL)
  {
      return NULL;
  }
  result->rows = rows;
  result->cols = cols;
  result->cords = (double**)malloc(rows*sizeof(double*));
  if (result->cords == NULL){
    free(result);
    return NULL;
  }
  for (i = 0; i < rows; i++){
    (result->cords)[i] = (double*)malloc(cols*sizeof(double));
    if ((result->cords)[i] == NULL){
      free_to_row(result, i);
    }
  }
  return result; 
}

Matrix* matrix_mul(Matrix* A, Matrix* B){
  if (A == NULL || B == NULL || A->cols != B->rows) {
    return NULL; // Error: matrices cannot be multiplied
  }
  Matrix* result = allocate_matrix(A->rows, B->cols);
  if (result == NULL) {
    return NULL; // Error: memory allocation failed
  }
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->cols; j++) {
      for (int k = 0; k < A->cols; k++) {
        result->cords[i][j] += A->cords[i][k] * B->cords[k][j];
      }
    }
  }
  return result;
}

void diag_pow(Matrix* X, int power){
  int i;
  for (i = 0; i < X->rows; i++){
    if (X->cords[i][i] != 0) { /* To avoid dividing by 0 */
        X->cords[i][i] = pow(X->cords[i][i], power);
    }
  }
}

Matrix* transpose(Matrix *X){
  Matrix* transposed = allocate_matrix(X->cols, X->rows);
  if (transposed == NULL) {
    return NULL; // Error: memory allocation failed
  }
  for (int i = 0; i < X->rows; i++) {
    for (int j = 0; j < X->cols; j++) {
      transposed->cords[j][i] = X->cords[i][j];
    }
  }
  return transposed;
}

void print_matrix(Matrix *X){
  for (int i = 0; i < X->rows; i++) {
    for (int j = 0; j < X->cols; j++) {
      printf("%.4f", X->cords[i][j]);
      if (j != X->cols - 1) {
        printf(","); // No comma at the last column
      }
    }
    if (i != X->rows - 1) {
      printf("\n"); // No new line at the last row
    }
  }
}

Matrix* file_to_matrix(char *filename){
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return NULL; // Error: file could not be opened
  }

  int rows = 0;
  int cols = 0;
  char line[1024];
  char *token;
  int firstLine = 1;

  while (fgets(line, sizeof(line), file)) {
    token = strtok(line, ",");
    if (firstLine) {
      cols = 0;
      while (token) {
        cols++;
        token = strtok(NULL, ",");
      }
      firstLine = 0;
    }
    rows++;
  }

  fclose(file);

  Matrix *matrix = allocate_matrix(rows, cols);
  if (matrix == NULL) {
    return NULL; // Error: memory allocation failed
  }

  file = fopen(filename, "r");
  int i = 0;
  while (fgets(line, sizeof(line), file)) {
    token = strtok(line, ",");
    int j = 0;
    while (token) {
      matrix->cords[i][j] = atof(token);
      token = strtok(NULL, ",");
      j++;
    }
    i++;
  }

  fclose(file);
  return matrix;
}

