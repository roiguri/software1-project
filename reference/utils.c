/*
 * This source file contains the implementation of utility functions declared in
 * the `utils.h` header file. For more information, please refer to the header file.
 */
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void error_handling(void)
{
    printf("An Error Has Occurred\n");
    exit(EXIT_FAILURE);
}