/**
 * This file provides implementations for various matrix operations.
 * For function prototypes and more detailed documentation, please refer to the corresponding
 * header file - matrix.h.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "matrix.h"
#include "file.h"
#include "utils.h"

/**
 * This function releases the memory used by a matrix structure, freeing the memory
 * for matrix elements and the matrix structure itself up to the given index. It is
 * used to handle partial freeing of matrix resources in specific scenarios.
 * This function is only used internally and is not declared in the public header file.
 */
void free_matrix_to_index(Matrix *mat, int index)
{
    int i;
    for (i = 0; i < index; i++)
    {
        free(mat->points[i]);
    }
    free(mat->points);
    free(mat);
}

Matrix *allocate_matrix(int length, int width)
{
    Matrix *matrix;
    int row;
    matrix = (Matrix *)malloc(sizeof(Matrix));
    if (matrix == NULL)
    {
        return NULL;
    }

    matrix->length = length;
    matrix->width = width;
    matrix->points = (double **)malloc(length * sizeof(double *));
    if (matrix->points == NULL)
    {
        free(matrix);
        return NULL;
    }
    for (row = 0; row < length; row++)
    {
        matrix->points[row] = (double *)malloc(width * sizeof(double));
        if (matrix->points[row] == NULL)
        {
            free_matrix_to_index(matrix, row); /* Free up to this point if allocation fails */
            return NULL;
        }
    }
    return matrix;
}

Matrix *parse_input_file(char *filename)
{
    FILE *file;
    int row = 0, col = 0, length, width;
    Matrix *matrix;
    char *line = NULL, *token;
    long read;
    size_t len = 0;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        error_handling(); /* Prints "An Error Has Occurred" and terminates */
    }
    count_rows_cols_in_file(file, &length, &width);
    rewind(file); /* Move back to the beginning of the file */

    matrix = allocate_matrix(length, width);
    if (matrix == NULL)
    {
        fclose(file);
        error_handling();
    }

    /* Read the data into the Matrix structure */
    while ((read = custom_getline(&line, &len, file)) != -1)
    {
        col = 0;
        token = strtok(line, ",");
        while (token != NULL)
        {
            matrix->points[row][col] = atof(token);
            col++;
            token = strtok(NULL, ",");
        }
        row++;
    }
    free(line); /* Free the memory allocated for the line buffer by custom_getline */
    fclose(file);
    return matrix;
}

double calc_squared_distance_two_vectors(double *first, double *second, int d)
{
    double squared_distance = 0.0;
    int i;
    for (i = 0; i < d; i++)
    {
        double diff = first[i] - second[i];
        squared_distance += diff * diff;
    }
    return squared_distance;
}

void free_matrix(Matrix *mat)
{
    if (mat != NULL)
        free_matrix_to_index(mat, mat->length); /* Passing the length - Completely freeing the matrix */
}

void print_matrix(Matrix *mat)
{
    int i, j;
    for (i = 0; i < mat->length; i++)
    {
        for (j = 0; j < mat->width; j++)
        {
            printf("%.4f", mat->points[i][j]);
            if (j != mat->width - 1)
            {
                printf(","); /* We don't want a comma at the last column*/
            }
        }
        if (i != mat->length - 1)
        { /* We don't want a new line at the last row */
            printf("\n");
        }
    }
}

double get_sum_of_array(double *arr, int size)
{
    double sum = 0.0;
    int i;
    for (i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return sum;
}

void diag_matrix_to_power_of(Matrix *mat, double power)
{
    int i;
    if (mat == NULL)
    {
        return;
    }
    /* Looping through the diagonal */
    for (i = 0; i < mat->length; i++)
    {
        if (mat->points[i][i] != 0)
        { /* To avoid dividing by 0 */
            mat->points[i][i] = pow(mat->points[i][i], power);
        }
    }
}


Matrix *matrix_multiply(Matrix *mat1, Matrix *mat2)
{
    Matrix *res;
    if (mat1 == NULL || mat2 == NULL)
    {
        return NULL;
    }
    res = allocate_matrix(mat1->length, mat2->width);
    matrix_multiply_in_place(res, mat1, mat2, NOT_TRANSPOSED, NOT_TRANSPOSED);
    return res;
}

Matrix *copy_matrix(Matrix *src)
{
    Matrix *dest;
    int i, j;
    if (src == NULL)
    {
        return NULL;
    }

    dest = allocate_matrix(src->length, src->width);
    if (dest == NULL)
    {
        return NULL;
    }

    for (i = 0; i < src->length; i++)
    {
        for (j = 0; j < src->width; j++)
        {
            dest->points[i][j] = src->points[i][j];
        }
    }

    return dest;
}