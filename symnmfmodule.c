#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include "symnmf.h"

/* Convertions*/
/* python list of lists to matrix*/
Matrix* PyObjectToMatrix(PyObject* cords){

}

/* C matrix struct to python list of lists*/
PyObject* PyObjectFromMatrix(Matrix* X){

}

/* Wrapper - sym */
static PyObject *sym_wrapper(PyObject *self, PyObject *args){
  Matrix *input, *c_result;
  PyObject *cords, *result;
  
  /* parse arguments */
  if (!PyArg_ParseTuple(args, "O", &points_obj))
  {
      return NULL;
  }
  input = PyObjectToMatrix(cords);
  if (input == NULL)
  {
      return NULL;
  }
  /* calculate */
  c_result = calc_sym(input);
  free_matrix(input);
  if (c_result == NULL)
  {
      return NULL;
  }
  result = PyObjectFromMatrix(c_result);
  free_matrix(c_result);
  return result;
}

/* Wrapper - ddg */
static PyObject *ddg_wrapper(PyObject *self, PyObject *args){
  Matrix *input, *sym, *c_result;
  PyObject *cords, *result;
  
  /* parse arguments */
  if (!PyArg_ParseTuple(args, "O", &cords))
  {
      return NULL;
  }
  input = PyObjectToMatrix(cords);
  if (input == NULL)
  {
      return NULL;
  }
  /* calculate */
  sym = calc_sym(input);
  c_result = calc_ddg(sym);
  free_matrix(input);
  free_matrix(sym);
  if (c_result == NULL)
  {
      return NULL;
  }
  result = PyObjectFromMatrix(c_result);
  free_matrix(c_result);
  return result;
}

/* Wrapper - norm */
static PyObject *norm_wrapper(PyObject *self, PyObject *args){
  Matrix *input, *sym, *ddg, *c_result;
  PyObject *cords, *result;
  
  /* parse arguments */
  if (!PyArg_ParseTuple(args, "O", &cords))
  {
      return NULL;
  }
  input = PyObjectToMatrix(cords);
  if (input == NULL)
  {
      return NULL;
  }
  /* calculate */
  sym = calc_sym(input);
  ddg = calc_ddg(sym);
  c_result = calc_norm(sym, ddg);
  free_matrix(input);
  free_matrix(sym);
  free_matrix(ddg);
  if (c_result == NULL)
  {
      return NULL;
  }
  result = PyObjectFromMatrix(c_result);
  free_matrix(c_result);
  return result;
}

/* Wrapper - symnmf */
static PyObject *norm_wrapper(PyObject *self, PyObject *args){
  Matrix *H_input, *W_input, *c_result;
  PyObject *H_cords, *W_cords *result;
  
  /* parse arguments */
  if (!PyArg_ParseTuple(args, "OO", &H_cords, &W_cords))
  {
      return NULL;
  }
  H_input = PyObjectToMatrix(H_cords);
  if (H_input == NULL)
  {
      return NULL;
  }
  W_input = PyObjectToMatrix(W_cords);
  if (W_input == NULL)
  {
      free_matrix(H_input)
      return NULL;
  }

  /* calculate */
  c_result = calc_symnmf(H_input, W_input);
  free_matrix(H_input);
  free_matrix(W_input);
  if (c_result == NULL)
  {
      return NULL;
  }
  result = PyObjectFromMatrix(c_result);
  free_matrix(c_result);
  return result;
}

/* Module's methods definitions */
static PyMethodDef symnmf_Methods[] = {
    {
        "sym",       /* name exposed to Python */
        sym_wrapper, /* C wrapper function */
        METH_VARARGS,
        "Computes the similarity matrix A from X" /* documentation */
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

/* Module definition */
static struct PyModuleDef symnmf_Module = {
    PyModuleDef_HEAD_INIT,
    "symnmfC",                                         /* name of module exposed to Python */
    "A C extension library for the symNMF algorithm.", /* module documentation*/
    -1,
    symnmf_Functions};

PyMODINIT_FUNC PyInit_symnmf(void)
{
    return PyModule_Create(&symnmf_Module);
}