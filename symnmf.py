import math
import numpy as np
import pandas as pd
import symnmfmodule  # This is the C extension module
import sys


def sym(mat):
  return symnmfmodule.sym(mat)


def ddg(mat):
  return symnmfmodule.ddg(mat)


def norm(mat):
  return symnmfmodule.norm(mat)


def symnmf(mat, k):
  H, W = initialize_H(mat, k)
  return symnmfmodule.symnmf(H, W)


def initialize_H(mat, k):
  # initialize W as the norm of mat and H with random values between 0 and 2*sqrt(m/k)
  W = norm(mat)
  m = np.mean(W)
  H = (np.random.uniform(0, 1, (len(mat), k)) * (2 * math.sqrt(m / k))).tolist()
  return H, W


def file_to_mat(file_name):
  # read the file and convert it to an array
  data_mat = pd.read_csv(file_name, header=None).to_numpy().tolist()
  return data_mat


def print_matrix(mat):
  for i in range(len(mat)):
    for j in range(len(mat[0]) -1):
      print("%.4f" % mat[i][j], end=",")
    print("%.4f" % mat[i][len(mat[0]) -1])


def main():
  # set seed:
  np.random.seed(1234)

  # Read command line arguments
  if len(sys.argv) != 4:
    print("An Error Has Occurred")
    return
  
  # initializing the variables
  k = int(sys.argv[1])
  goal = str(sys.argv[2])
  file_name = str(sys.argv[3])
  
  # Read data from file
  X = file_to_mat(file_name)
  
  # Action based on user goal input:   
  if (goal == 'symnmf'):
    print_matrix(symnmf(X, k))
  
  elif (goal == 'sym'):
    print_matrix(sym(X))

  elif (goal == 'ddg'):
    print_matrix(ddg(X))

  elif (goal == 'norm'):
    print_matrix(norm(X))

  else:
    print("An Error Has Occurred")
    return
  

if __name__ == "__main__":
  main()