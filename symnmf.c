#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "symnmf.h"

#define EPSILON 1e-4
#define MAX_ITER 300

/* Function to read csv file */
double** readFile(char* filename) {
  return; 
}

/* Function to print output */
void createOutput(double** matrix){
  return;
}

/* Function to calculate matrix shape */
int* getShape(double** matrix){
  return;
}

/* Matrix Multiplication */
int** multiply_matrices(double** A, double** B){
  return;
}

/* Transpose Matrix */
int** transpose_matrix(double** A){
  return;
}

/* Function to calculate Euclidean distance between two points */
double euclidean_distance(double *x, double *y, int d) {
  double sum = 0.0;
  int i;
  for (i = 0; i < d; i++) {
    sum += pow(x[i] - y[i], 2);
  }
  return sum;
}

/* Function to calculate similarity matrix */
double** calculate_similarity_matrix(double **X, int N, int d) {
  int i, j;
  double **A = malloc(N * sizeof(double*));
  for (i = 0; i < N; i++) {
    A[i] = calloc(N, sizeof(double));
    for (j = 0; j < N; j++) {
      if (i != j) {
        A[i][j] = exp(-euclidean_distance(X[i], X[j], d)/2);
      }
    }
  }
  return A;
}

/* Function to calculate diagonal degree matrix */
double** calculate_diagonal_degree_matrix(double **A, int N) {
  int i, j;
  double **D = malloc(N * sizeof(double*));
  for (i = 0; i < N; i++) {
    D[i] = calloc(N, sizeof(double));
    for (j = 0; j < N; j++) {
      D[i][i] += A[i][j];
    }
  }
  return D;
}

/* Function to calculate normalized similarity matrix */
double** calculate_normalized_similarity(double **A, double **D, int N) {
  int i, j;
  double **W = malloc(N * sizeof(double*));
  for (i = 0; i < N; i++) {
    W[i] = malloc(N * sizeof(double));
    for (j = 0; j < N; j++) {
      W[i][j] = A[i][j] * sqrt(D[i][i]) * sqrt(D[j][j]);
    }
  }
  return W;
}

/* Function to calculate symnmf */
double** calculate_symnmf(double **H, double **W, int N, int d, int k){
  return;
}

/* Handle different goals */
void accomplishGoal(char *goal, char *filename) {
    /* Read data from file */
    double **X = readFile(filename);
    /* get matrix shape */
    int* shape = getShape(X);
    int N = shape[0];
    int d = shape[1];

    if (strcmp(goal, "sym") == 0) {
        double **A = calculate_similarity_matrix(X, N, d);
        /* Print A */
        createOutput(A);
    } else if (strcmp(goal, "ddg") == 0) {
        double **A = calculate_similarity_matrix(X, N, d);
        double **D = calculate_diagonal_degree_matrix(A, N);
        /* Print D */
        createOutput(D);
    } else if (strcmp(goal, "norm") == 0) {
        double **A = calculate_similarity_matrix(X, N, d);
        double **D = calculate_diagonal_degree_matrix(A, N);
        double **W = calculate_normalized_similarity(A, D, N);
        /* Print W */
        createOutput(W);
    } else {
        printf("An Error Has Occurred\n");
        exit(1);
    }

    /* Free allocated memory */ 
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("An Error Has Occurred\n");
    return 1;
  }

  char *goal = argv[1];
  char *filename = argv[2];

  accomplishGoal(goal, filename);

  return 0;
}