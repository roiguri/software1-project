/**
 * This header file defines the `Matrix` structure and declares various functions for working with matrices,
 * such as allocation, freeing memory, multiplication (with and without transposition), and more.
 */

#ifndef MATRIX_H
#define MATRIX_H

/**
 * This structure holds a 2D array of doubles and its dimensions (rows and cols)
 */
typedef struct {
    double **cords;
    int rows;
    int cols;
} Matrix;

void free_matrix(Matrix *A);
void free_matrix2(Matrix *A, Matrix *B);
void free_matrix3(Matrix *A, Matrix *B, Matrix *C);
Matrix* allocate_matrix(int rows, int cols);
Matrix* matrix_mul(Matrix* A, Matrix* B); // memory allocation & error handling for return matrix
void diag_pow(Matrix* X, int power);
Matrix* transpose(Matrix *X);
void print_matrix(Matrix *X);
Matrix* file_to_matrix(char *filename);

#endif