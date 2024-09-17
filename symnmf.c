#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "symnmf.h"
#include "mat_utils.h"

#define EPSILON 1e-4
#define MAX_ITER 300

/* Function to calculate Euclidean distance between two points */
double euclidean_distance(double *x, double *y, int d) {
  double sum = 0.0;
  int i;
  for (i = 0; i < d; i++) {
    sum += pow(x[i] - y[i], 2);
  }
  return sum;
}

/* Functions to calculate similarity matrix */
Matrix* calc_sym(Matrix *X) {
  int i, j;
  double** cords = X->cords;
  if (X == NULL){
    return NULL;
  }
  Matrix *A = allocate_matrix(X->rows, X->rows);
  if (A == NULL){
    return NULL;
  }
  for (i = 0; i < X->rows; i++) {
    for (j = 0; j < X->rows; j++) {
      if (i == j) {
        (A->cords)[i][j] = 0;
      } else if (i < j){
        (A->cords)[i][j] = exp(-euclidean_distance(cords[i], cords[j], X->cols)/2);
      } else {
        /* i > j, the matrix is symetric */
        (A->cords)[i][j] = (A->cords)[j][i];
      }
    }
  }
  return A;
}

void sym(Matrix *X) {
  Matrix *sym_mat = calc_sym(X);
  if (sym_mat == NULL)
  {
    free_matrix(X);
    error_handling();
  }
  print_matrix(sym_mat);
  free_matrix(sym_mat);
}



/* Function to calculate diagonal degree matrix */
Matrix* calc_ddg(Matrix *A) {
  int i, j;
  if (A == NULL){
    return NULL;
  }
  Matrix *D = allocate_matrix(A->rows, A->rows);
  if (D == NULL){
    return NULL;
  }
  for (i = 0; i < A->rows; i++) {
    for (j = 0; j < A->rows; j++) {
      if (i != j){
        (D->cords)[i][j] = 0;
      }
      (D->cords)[i][i] += (A->cords)[i][j];
    }
  }
  return D;
}

void ddg(Matrix *X) {
    Matrix *D, *A;
    A = calc_sym(X);
    D = calc_ddg(A);
    free_matrix(A);
    if (D == NULL)
    {
        free_matrix(X);
        error_handling();
    }
    print_matrix(D);
    free_matrix(D);
}

/* Function to calculate normalized similarity matrix */
Matrix* calc_norm(Matrix *A, Matrix *D) {
  int i, j;
  Matrix *first_mul, *W;
  if ((A == NULL) || (D == NULL)){
    return NULL;
  }
  diag_pow(D, -0.5);
  first_mul = matrix_mul(D, A);
  W = matrix_mul(first_mul, D);

  return W;
}

void norm(Matrix *X){
  Matrix *W, *D, *A;
  A = calc_sym(X);
  if (A == NULL){
    free_matrix(X);
    error_handling();
  }
  D = calc_ddg(A);
  if (D == NULL){
    free_matrix(X);
    free_matrix(A);
    error_handling();
  }
  W = calc_norm(A, D);
  free_matrix(A);
  free_matrix(D);
  if (W == NULL)
  {
      free_matrix(X);
      error_handling();
  }
  print_matrix(W);
  free_matrix(W);
}

/* Function to calculate symnmf */
double** symnmf(double **H, double **W, int N, int d, int k){
  return;
}

int main(int argc, char *argv[]) {
  Matrix *matrix;
  if (argc != 3) {
    printf("An Error Has Occurred\n");
    return 1;
  }

  char *goal = argv[1];
  char *filename = argv[2];

  matrix = file_to_matrix(filename);
  if (matrix == NULL)
  {
    printf("An Error Has Occurred\n");
    return 1;
  }

  if (strcmp(goal, "sym") == 0)
  {
    sym(matrix);
  }
  else if (strcmp(goal, "ddg") == 0)
  {
    ddg(matrix);
  }
  else if (strcmp(goal, "norm") == 0)
  {
    norm(matrix);
  }
  free_matrix(matrix);
  return 0;
}