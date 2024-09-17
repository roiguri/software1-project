/**
 * This header file defines the `Matrix` structure and declares various functions for working with matrices,
 * such as allocation, freeing memory, multiplication (with and without transposition), and more.
 */

#ifndef MATRIX_H
#define MATRIX_H

/**
 * This structure holds a 2D array of doubles and its dimensions (rows and cols).
 */
typedef struct {
    double **cords;
    int rows;
    int cols;
} Matrix;

free_matrix;
allocate_matrix;
matrix_mul;
diag_pow;
print_matrix;
file_to_matrix;

#endif