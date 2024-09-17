/**
 * This header file defines the `Matrix` structure and declares various functions for working with matrices,
 * such as allocation, freeing memory, multiplication (with and without transposition), and more.
 */

#ifndef MATRIX_H
#define MATRIX_H
#define TRANSPOSED 1
#define NOT_TRANSPOSED 0
/**
 * This structure holds a 2D array of doubles (the matrix data) and its dimensions (length and width).
 */
typedef struct {
    double **points;
    int length;
    int width;
} Matrix;

/**
 * Allocates memory for a matrix with the given dimensions.
 * If an error occurs, NULL is returned.
 */
Matrix *allocate_matrix(int length, int width);

/**
 * Frees the memory allocated for a matrix.
 */
void free_matrix(Matrix *mat);

/**
 * Prints the contents of a matrix according to a sepcific format.
 */
void print_matrix(Matrix *mat);

/**
 * Multiplies two matrices and returns a new matrix with the result.
 * If an error occurs, NULL is returned.
 */
Matrix *matrix_multiply(Matrix *mat1, Matrix *mat2);

/**
 * Calculates the squared distance between two vectors.
 */
double calc_squared_distance_two_vectors(double *first, double *second, int d);

/**
 * Computes the sum of elements in an array.
 */
double get_sum_of_array(double *arr, int size);

/**
 * Raises each element in the diagonal of a matrix to a specified power.
 */
void diag_matrix_to_power_of(Matrix *mat, double power);

/**
 * Parses a matrix from an input file according to the following format - 
 * each point in its own line while its entries are separated by a comma.
 * If an error occurs, the program exits!
 */
Matrix *parse_input_file(char *filename);

/**
 * Creates a copy of a matrix.
 * If an error occurs, NULL is returned.
 */
Matrix* copy_matrix(Matrix *src);

#endif /* MATRIX_H */