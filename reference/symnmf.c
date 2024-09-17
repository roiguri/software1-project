/**
 * This project is divided into several modules to enhance organization and readability.
 *
 * - utils.c: Contains the error handling function.
 * - matrix.c: Contains functions related to matrix operations.
 * - file.c: Handles file input operations.
 *
 * For more detailed information, please refer to the documentation within each respective module.
 */
/**
 * This file contains the main implementation of the SymNMF algorithm.
 * It includes functions for calculating symmetric matrices, diagonal degree matrices,
 * and normalized matrices, as well as the SymNMF algorithm itself. Additionally,
 * it includes a main function for executing the algorithm, as specified in the requirements PDF.
 *
 * While we could have split the functions into separate files and kept only the main function
 * in symnmf.c, we decided in this case to keep everything in a single file for simplicity.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "symnmf.h"
#include "utils.h"
#define EPSILON 0.0001
#define MAX_ITER 300
#define D_POWER -0.5
#define BETA 0.5

void sym(Matrix *mat);
void ddg(Matrix *mat);
void norm(Matrix *mat);
int update_h_by_rule(Matrix *h_next, Matrix *h_curr, Matrix *wh_mat, Matrix *hhTh_mat);
int converged(Matrix *h_next, Matrix *h_curr);
double get_squared_frob_norm_of_difference(Matrix *h_next, Matrix *h_curr);

Matrix *calc_sym(Matrix *inp_mat)
{
    int i, j;
    Matrix *sym;
    if (inp_mat == NULL)
    {
        return NULL;
    }
    sym = allocate_matrix(inp_mat->length, inp_mat->length);
    if (sym == NULL)
    {
        return NULL;
    }
    for (i = 0; i < sym->length; i++)
    {
        for (j = 0; j < sym->width; j++)
        {
            if (i == j)
            {
                sym->points[i][j] = 0;
            }
            else if (i < j)
            {
                sym->points[i][j] = exp((-0.5 * calc_squared_distance_two_vectors(inp_mat->points[i], inp_mat->points[j], inp_mat->width))); /* According to the defintion. */
            }
            else
            { /* i > j, symmetric, we can go to the symmetric elemnent in the matrix */
                sym->points[i][j] = sym->points[j][i];
            }
        }
    }
    return sym;
}

Matrix *calc_ddg(Matrix *sym_mat)
{
    int i, j;
    Matrix *ddg;

    if (sym_mat == NULL)
    {
        return NULL;
    }
    ddg = allocate_matrix(sym_mat->length, sym_mat->width);
    if (ddg == NULL)
    {
        return NULL;
    }
    for (i = 0; i < ddg->length; i++)
    {
        for (j = 0; j < ddg->width; j++)
        {
            if (i == j) /* A diagonal matrix */
            {
                ddg->points[i][j] = get_sum_of_array(sym_mat->points[i], sym_mat->width); /* The sum of each row in the sym matrix, as defined*/
            }
            else
            {
                ddg->points[i][j] = 0;
            }
        }
    }

    return ddg;
}

Matrix *calc_norm(Matrix *sym_mat)
{
    Matrix *ddg_mat, *norm_mat, *first_mul;
    ddg_mat = calc_ddg(sym_mat);
    diag_matrix_to_power_of(ddg_mat, D_POWER);
    first_mul = matrix_multiply(ddg_mat, sym_mat);
    norm_mat = matrix_multiply(first_mul, ddg_mat);
    free_matrix(ddg_mat);
    free_matrix(first_mul);
    return norm_mat;
}


Matrix *calc_symnmf_python(Matrix *h_init_matrix, Matrix *w_matrix)
{
    /*
     * Even if one of the matrices is null, we will catch that later in the following functions
     * (because we check there if the inputs are null), here we will catch that in the return status of update_h
     * This pattern is consistently followed throughout this file.
     */
    int iterations = 0, update_status;
    Matrix *h_next = NULL, *h_curr = NULL, *wh_mat = NULL, *hhT_mat = NULL, *hhTh_mat = NULL, *temp;

    h_next = allocate_matrix(h_init_matrix->length, h_init_matrix->width);
    h_curr = copy_matrix(h_init_matrix);
    wh_mat = allocate_matrix(h_init_matrix->length, h_init_matrix->width);
    hhT_mat = allocate_matrix(h_init_matrix->length, h_init_matrix->length);
    hhTh_mat = allocate_matrix(h_init_matrix->length, h_init_matrix->width);

    while (iterations < MAX_ITER)
    {
        /* Calculate all necessary matrices */
        matrix_multiply_in_place(wh_mat, w_matrix, h_curr, NOT_TRANSPOSED, NOT_TRANSPOSED);
        matrix_multiply_in_place(hhT_mat, h_curr, h_curr, NOT_TRANSPOSED, TRANSPOSED);
        matrix_multiply_in_place(hhTh_mat, hhT_mat, h_curr, NOT_TRANSPOSED, NOT_TRANSPOSED);

        update_status = update_h_by_rule(h_next, h_curr, wh_mat, hhTh_mat);

        if (!update_status || converged(h_next, h_curr)) /* update_status - if the update has failed*/
        {
            break;
        }

        /* Swapping pointers */
        temp = h_curr;
        h_curr = h_next;
        h_next = temp;
        iterations++;
    }

    free_matrix(h_curr);
    free_matrix(wh_mat);
    free_matrix(hhT_mat);
    free_matrix(hhTh_mat);
    return h_next;
}

int update_h_by_rule(Matrix *h_next, Matrix *h_curr, Matrix *wh_mat, Matrix *hhTh_mat)
{
    int i, j;
    if (h_next == NULL || h_curr == NULL || wh_mat == NULL || hhTh_mat == NULL)
    {
        return 0; /* Error */
    }

    for (i = 0; i < h_next->length; i++)
    {
        for (j = 0; j < h_next->width; j++)
        {
            if (hhTh_mat->points[i][j] == 0)
            {
                return 0; /* It is defined to divide by that value, so a value of 0 will result in an error. */
            }
            h_next->points[i][j] = h_curr->points[i][j] * (1 - BETA + BETA * (wh_mat->points[i][j] / hhTh_mat->points[i][j]));
        }
    }
    return 1;
}

int converged(Matrix *h_next, Matrix *h_curr)
{
    return get_squared_frob_norm_of_difference(h_next, h_curr) < EPSILON;
}

/*
 * This function is not general-purpose. For time and space efficiency,
 * I decided to implement it here rather than in matrix.h. As mentioned,
 * it is specific to this use case.
 */
double get_squared_frob_norm_of_difference(Matrix *h_next, Matrix *h_curr)
{
    double difference, sum = 0.0;
    int i, j;
    if (h_next->length != h_curr->length || h_next->width != h_curr->width)
    {
        return -1.0; /* error */
    }

    for (i = 0; i < h_next->length; i++)
    {
        for (j = 0; j < h_next->width; j++)
        {
            difference = h_next->points[i][j] - h_curr->points[i][j];
            sum += difference * difference;
        }
    }

    return sum;
}

/**
 * The functions below simply call the corresponding `calc_*` function to perform the necessary
 * matrix computation and then print the resulting matrix.
 * They are used and created for the main function
 */

void sym(Matrix *mat)
{
    Matrix *sym_mat = calc_sym(mat);
    if (sym_mat == NULL)
    {
        free_matrix(mat);
        error_handling();
    }
    print_matrix(sym_mat);
    free_matrix(sym_mat);
}

void ddg(Matrix *mat)
{
    /*
     * Even if sym_mat is null, we will catch that later in the following functions
     * (because we check there if the inputs are null).
     * This pattern is consistently followed throughout this file, as mentioned earlier.
     */
    Matrix *ddg_mat, *sym_mat;
    sym_mat = calc_sym(mat);
    ddg_mat = calc_ddg(sym_mat);
    free_matrix(sym_mat);
    if (ddg_mat == NULL)
    {
        free_matrix(mat);
        error_handling();
    }
    print_matrix(ddg_mat);
    free_matrix(ddg_mat);
}

void norm(Matrix *mat)
{
    Matrix *norm_mat, *sym_mat;
    sym_mat = calc_sym(mat);
    norm_mat = calc_norm(sym_mat);
    free_matrix(sym_mat);
    if (norm_mat == NULL)
    {
        free_matrix(mat);
        error_handling();
    }
    print_matrix(norm_mat);
    free_matrix(norm_mat);
}

int main(int argc, char *argv[])
{
    Matrix *matrix;
    if (argc != 3)
    {
        error_handling();
    }
    matrix = parse_input_file(argv[2]);
    if (matrix == NULL)
    {
        error_handling();
    }

    if (strcmp(argv[1], "sym") == 0)
    {
        sym(matrix);
    }
    else if (strcmp(argv[1], "ddg") == 0)
    {
        ddg(matrix);
    }
    else if (strcmp(argv[1], "norm") == 0)
    {
        norm(matrix);
    }
    free_matrix(matrix);

    return EXIT_SUCCESS;
}