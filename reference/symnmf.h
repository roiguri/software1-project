/**
 * This header file declares functions for calculating and processing matrices 
 * for Symmetric Non-negative Matrix Factorization (SymNMF). The functions 
 * include operations as detailed in the requirements PDF file.
 */

#ifndef SYM_NMF_H
#define SYM_NMF_H

#include "matrix.h"

/**
 * Computes the similarity matrix based on the provided input matrix.
 * Returns NULL in case of an error.
 */
Matrix *calc_sym(Matrix *inp_mat);

/**
 * Computes the diagonal degree matrix from the given similarity matrix.
 * Returns NULL in case of an error.
 */
Matrix *calc_ddg(Matrix *sym_mat);

/**
 * Computes the normalized similarity matrix based on the given similarity matrix.
 * Returns NULL in case of an error.
 */
Matrix *calc_norm(Matrix *sym_mat);

/**
 * Performs SymNMF factorization given initial matrices for H and W.
 * Returns NULL in case of an error.
 */
Matrix *calc_symnmf_python(Matrix *h_init_matrix, Matrix *w_matrix);

#endif /* SYM_NMF_H */