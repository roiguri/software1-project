#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include "symnmf.h"
/*
 * This C extension module is designed to interface with Python for the SymNMF algorithm.
 * It provides functions for calculating the similarity matrix, diagonal degree matrix,
 * normalized similarity matrix, and the SymNMF decomposition matrix.
 *
 * The implementation is based on examples and patterns discussed in lectures, specifically
 * focusing on how to structure and write a Python C extension module.
 *
 * Note: We could not name this C module as 'symnmf' due to a naming conflict with 'symnmf'
 * in 'analysis.py'. Instead, the module is named 'symnmfC' to avoid this interference.
 */


/* Function to convert a Python list of lists to a matrix pointer */
Matrix *convertToMatrixOfFloats(PyObject *obj)
{
    int i, j;
    Matrix *matrix;
    PyObject *row, *elem;
    Py_ssize_t rows, cols;

    /* Get the number of rows in the Python list */
    rows = PyObject_Length(obj);
    if (rows <= 0)
    {
        return NULL;
    }

    matrix = allocate_matrix(rows, PyObject_Length(PyList_GetItem(obj, 0)));

    /* Iterate over the rows of the Python list */
    for (i = 0; i < rows; ++i)
    {
        /* Get the sublist at index i */
        row = PyList_GetItem(obj, i);
        /* Get the number of columns in the sublist */
        cols = PyObject_Length(row);
        if (cols < 0)
        {
            free_matrix(matrix);
            return NULL;
        }
        /* Iterate over the elements of the sublist and convert them to doubles */
        for (j = 0; j < cols; ++j)
        {
            elem = PyList_GetItem(row, j);
            matrix->points[i][j] = PyFloat_AsDouble(elem);
        }
    }

    return matrix;
}
/** Function to clean up a partially constructed list.
 * Although we didn't cover this in the lecture and I haven't checked it much,
 * but i decided to include it just in case.
 * Freeing the list should take care of freeing the floats, to my understanding.
 */
void cleanup_partial_list(PyObject *pyList, int completed_rows)
{
    int k;
    for (k = 0; k < completed_rows; k++)
    {
        Py_DECREF(PyList_GET_ITEM(pyList, k));
    }
    Py_DECREF(pyList);
}

PyObject *convertToPythonList(double **arr, int rows, int cols)
{
    PyObject *pyList, *rowList, *pyFloat;
    int i, j;

    pyList = PyList_New(rows); /* Create a Python list to hold rows*/
    if (!pyList)
    {
        return NULL; /* Memory allocation failed*/
    }

    for (i = 0; i < rows; i++)
    {
        rowList = PyList_New(cols); /* Create a Python list to hold elements of each row*/
        if (!rowList)
        {
            cleanup_partial_list(pyList, i); /* Clean up the previously created lists*/
            return NULL;                     /* Memory allocation failed*/
        }
        for (j = 0; j < cols; j++)
        {
            pyFloat = PyFloat_FromDouble(arr[i][j]); /* Convert double to Python float*/
            if (!pyFloat)
            {
                Py_DECREF(rowList);
                cleanup_partial_list(pyList, i); /* Clean up previously added rows*/
                return NULL;                     /* Memory allocation failed*/
            }
            PyList_SET_ITEM(rowList, j, pyFloat); /* Add float to row list */
        }
        PyList_SET_ITEM(pyList, i, rowList); /* Add row list to the overall matrix list */
    }
    return pyList;
}
// wrapper function for sym
static PyObject *sym_wrapper(PyObject *self, PyObject *args)
{
    Matrix *inp_matrix, *sym_result;
    PyObject *points_obj, *result;

    if (!PyArg_ParseTuple(args, "O", &points_obj))
    {
        return NULL;
    }

    inp_matrix = convertToMatrixOfFloats(points_obj);
    if (inp_matrix == NULL)
    {
        return NULL;
    }

    sym_result = calc_sym(inp_matrix);
    free_matrix(inp_matrix);
    if (sym_result == NULL)
    {
        return NULL;
    }
    result = convertToPythonList(sym_result->points, sym_result->length, sym_result->width);
    free_matrix(sym_result);
    return result;
}

// wrapper function for ddg
static PyObject *ddg_wrapper(PyObject *self, PyObject *args)
{
    Matrix *inp_matrix, *ddg_result, *sym_result;
    PyObject *points_obj, *result;

    if (!PyArg_ParseTuple(args, "O", &points_obj))
    {
        return NULL;
    }

    inp_matrix = convertToMatrixOfFloats(points_obj);
    if (inp_matrix == NULL)
    {
        return NULL;
    }

    sym_result = calc_sym(inp_matrix);
    ddg_result = calc_ddg(sym_result);
    free_matrix(inp_matrix);
    free_matrix(sym_result);
    if (ddg_result == NULL)
    {
        return NULL;
    }
    result = convertToPythonList(ddg_result->points, ddg_result->length, ddg_result->width);
    free_matrix(ddg_result);
    return result;
}

// wrapper function for norm
static PyObject *norm_wrapper(PyObject *self, PyObject *args)
{
    Matrix *inp_matrix, *norm_result, *sym_result;
    PyObject *points_obj, *result;

    if (!PyArg_ParseTuple(args, "O", &points_obj))
    {
        return NULL;
    }

    inp_matrix = convertToMatrixOfFloats(points_obj);
    if (inp_matrix == NULL)
    {
        return NULL;
    }

    sym_result = calc_sym(inp_matrix);
    norm_result = calc_norm(sym_result);
    free_matrix(inp_matrix);
    free_matrix(sym_result);
    if (norm_result == NULL)
    {
        return NULL;
    }
    result = convertToPythonList(norm_result->points, norm_result->length, norm_result->width);
    free_matrix(norm_result);
    return result;
}

// wrapper function for symnmf
static PyObject *symnmf_wrapper(PyObject *self, PyObject *args)
{
    Matrix *h_init_matrix, *w_matrix, *symnmf_result;
    PyObject *h_obj, *w_obj, *result;

    if (!PyArg_ParseTuple(args, "OO", &h_obj, &w_obj))
    {
        return NULL;
    }

    h_init_matrix = convertToMatrixOfFloats(h_obj);
    if (h_init_matrix == NULL)
    {
        return NULL;
    }
    w_matrix = convertToMatrixOfFloats(w_obj);
    if (w_matrix == NULL)
    {
        free_matrix(h_init_matrix);
        return NULL;
    }

    symnmf_result = calc_symnmf_python(h_init_matrix, w_matrix);
    free_matrix(h_init_matrix);
    free_matrix(w_matrix);
    if (symnmf_result == NULL)
    {
        return NULL;
    }
    result = convertToPythonList(symnmf_result->points, symnmf_result->length, symnmf_result->width);
    free_matrix(symnmf_result);
    return result;
}

/* module's function table */
static PyMethodDef symnmfC_FunctionsTable[] = {
    {
        "sym",       /* name exposed to Python */
        sym_wrapper, /* C wrapper function */
        METH_VARARGS,
        "Forms the similarity matrix A from X" /* documentation */
    },
    {
        "ddg",       /* name exposed to Python */
        ddg_wrapper, /* C wrapper function */
        METH_VARARGS,
        "Computes the Diagonal Degree Matrix" /* documentation */
    },
    {
        "norm",       /* name exposed to Python */
        norm_wrapper, /* C wrapper function */
        METH_VARARGS,
        "Computes the normalized similarity W" /* documentation */
    },
    {
        "symnmf",       /* name exposed to Python */
        symnmf_wrapper, /* C wrapper function */
        METH_VARARGS,
        "Finds the decomposition matrix H" /* documentation */
    },
    {NULL, NULL, 0, NULL}};

// modules definition
static struct PyModuleDef symnmfC_Module = {
    PyModuleDef_HEAD_INIT,
    "symnmfC",                                         /* name of module exposed to Python */
    "A C extension library for the symNMF algorithm.", /* module documentation*/
    -1,
    symnmfC_FunctionsTable};

PyMODINIT_FUNC PyInit_symnmfC(void)
{
    return PyModule_Create(&symnmfC_Module);
}