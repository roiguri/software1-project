import numpy as np
import symnmf  # This is the C extension module
import sys



def main():
  # set seed:
  np.random.seed(1234)

  # Read command line arguments
  if len(sys.argv) != 4:
    print("An Error Has Occurred")
    return
  
  k = int(sys.argv[1])
  goal = sys.argv[2]
  file_name = sys.argv[3]
  
  # Read data from file
  try:
    X = np.loadtxt(file_name, delimiter=",")
  except:
    print("An Error Has Occurred")
    return
  
  # Action based on user goal input: 
  if goal == "symnmf":
    # Initialize H
    N, _ = X.shape
    W = symnmf.norm(X)
    m = np.mean(W)
    H = np.random.uniform(0, 2 * np.sqrt(m / k), (N, k))
    
    # Calculate symnmf
    result = symnmf.symnmf(H, W)
  elif goal == "sym":
    result = symnmf.sym(X)
  elif goal == "ddg":
    result = symnmf.ddg(X)
  elif goal == "norm":
    result = symnmf.norm(X)
  else:
    print("An Error Has Occurred")
    return
  
  # Output result
  np.savetxt(sys.stdout, result, fmt='%.4f', delimiter=',')

if __name__ == "__main__":
  main()