#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "symnmf.h"
#include "mat_utils.h"
#include "utils.h"

#define EPSILON 0.0001
#define MAX_ITER 300
#define BETA 0.5

/* Function to calculate Squared Euclidean distance between two cord vectors */
double squared_euclidean_distance(double *x, double *y, int d) {
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
  Matrix *A;
  double** cords = X->cords;
  if (X == NULL){
    return NULL;
  }
  A = allocate_matrix(X->rows, X->rows);
  if (A == NULL){
    return NULL;
  }
  for (i = 0; i < X->rows; i++) {
    for (j = 0; j < X->rows; j++) {
      if (i == j) {
        (A->cords)[i][j] = 0;
      } else if (i < j){
        (A->cords)[i][j] = exp(-squared_euclidean_distance(cords[i], cords[j], X->cols)/2);
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
    error_has_occured();
  }
  print_matrix(sym_mat);
  free_matrix(sym_mat);
}



/* Function to calculate diagonal degree matrix */
Matrix* calc_ddg(Matrix *A) {
  int i, j;
  Matrix *D;
  if (A == NULL){
    return NULL;
  }
  D = allocate_matrix(A->rows, A->rows);
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
        error_has_occured();
    }
    print_matrix(D);
    free_matrix(D);
}

/* Function to calculate normalized similarity matrix */
Matrix* calc_norm(Matrix *A, Matrix *D) {
  Matrix *first_mul, *W;
  if ((A == NULL) || (D == NULL)){
    return NULL;
  }
  diag_pow(D, -0.5);
  first_mul = matrix_mul(D, A);
  if (first_mul == NULL){
    return NULL;
  }
  W = matrix_mul(first_mul, D);
  if (W == NULL){
    free_matrix(first_mul);
    return NULL;
  }
  return W;
}

void norm(Matrix *X){
  Matrix *W, *D, *A;
  A = calc_sym(X);
  if (A == NULL){
    free_matrix(X);
    error_has_occured();
  }
  D = calc_ddg(A);
  if (D == NULL){
    free_matrix2(X, A);
    error_has_occured();
  }
  W = calc_norm(A, D);
  free_matrix2(A, D);
  if (W == NULL)
  {
    free_matrix(X);
    error_has_occured();
  }
  print_matrix(W);
  free_matrix(W);
}

double squared_frobenius_norm(Matrix *H, Matrix *H_next){
  double sum = 0.0;
  int i, j;
  for (i = 0; i < H->rows; i++){
    for (j = 0; j < H->cols; j++){
      sum += pow((H_next->cords)[i][j]-(H->cords)[i][j], 2);
    }
  }
  return sum;
}

int check_convergence(Matrix *H, Matrix *H_next){
  double sum = squared_frobenius_norm(H, H_next);
  return (sum < EPSILON);
}

Matrix* update_H(Matrix *H, Matrix *W){
  Matrix *numerator, *denominator, *temp;
  int i, j;
  double h, numer_val, denom_val;
  Matrix *H_next = allocate_matrix(H->rows, H->cols);
  if (H_next == NULL){
    return NULL;
  }
  
  /* Calculate Numerator: */
  numerator = matrix_mul(W, H);
  if (numerator == NULL){
    free_matrix(H_next);
    return NULL;
  }
  
  /* Calculate Denominator: */
  temp = matrix_mul(H, transpose(H));
  if (temp == NULL){
    free_matrix2(H_next, numerator);
    return NULL;
  }

  denominator = matrix_mul(temp, H);
  if (denominator == NULL){
    free_matrix3(H_next, numerator, temp);
    return NULL;
  }
  free_matrix(temp);
  
  /* update H: */
  for (i = 0; i < H->rows; i++){
    for (j = 0; j < H->cols; j++){
      h = (H->cords)[i][j];
      numer_val = (numerator->cords)[i][j];
      denom_val = (denominator->cords)[i][j];
      (H_next->cords)[i][j] = h*(1-BETA+BETA*(numer_val/denom_val));
    }
  }
  free_matrix(H);
  free_matrix(W);
  return H_next;
}

/* Function to calculate symnmf */
Matrix* symnmf(Matrix *H, Matrix *W){
  int i;
  Matrix *H_next, *H_prev;

  if (H == NULL){
    free_matrix(W);
    error_has_occured();
  } 
  else if (W == NULL){
    free_matrix(H);
    error_has_occured();
  }
  for (i=0; i < MAX_ITER; i++){
    H_next = update_H(H, W);
    if (H_next == NULL){
      free_matrix(H);
      free_matrix(W);
      error_has_occured();
    }
    if(check_convergence(H, H_next)){
      free_matrix(H);
      break;
    }
    H_prev = H;
    H = H_next;
    free_matrix(H_prev);
  }
  return H_next;
}

void test(Matrix* matrix){
  diag_pow(matrix,2);
  print_matrix(matrix);
}

int main(int argc, char *argv[]) {
  Matrix *matrix;
  char *goal, *filename;
  if (argc != 3) {
    error_has_occured();
  }

  goal = argv[1];
  filename = argv[2];

  matrix = file_to_matrix(filename);
  if (matrix == NULL)
  {
    error_has_occured();
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
  else if (strcmp(goal, "test") == 0)
  {
    test(matrix);
  }
  free_matrix(matrix);
  return 0;
}