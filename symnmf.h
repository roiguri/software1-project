#ifndef SYMNMF_H
#define SYMNMF_H

#include "mat_utils.h"

Matrix* calc_sym(Matrix *X);
Matrix* calc_ddg(Matrix *A);
Matrix* calc_norm(Matrix *A, Matrix *D);
Matrix* symnmf(Matrix *H, Matrix *W);


#endif