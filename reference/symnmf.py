"""
This file is a pretty straightforward Python script that utilizes the symnmfC module, which we implemented in C.
It wraps the functionality of symnmfC within a Python class, providing a interface to interact with the SymNMF algorithm.
"""
import sys
import numpy as np
import pandas as pd
import math
import symnmfC


class SymnmfPython:
    SEED = 0
    """
    Attributes:
    - SEED (int): A class-level attribute that sets the random seed for reproducibility. Default is 0.
    - __points (list of lists): An instance-level attribute that stores the data points loaded from the input file. Initialized by the `set_data()` method as a list of lists of numerical values.
    """

    def __init__(self, file_name):
        np.random.seed(SymnmfPython.SEED)
        self.set_data(file_name) # Creates the only instance field - __points

    @classmethod
    def set_data(self, file_name):
        # read the file and convert it to an array
        Points_DataFrame = pd.read_csv(file_name, header=None)
        self.__points = Points_DataFrame.to_numpy().tolist()
    
    
    @classmethod
    def get_data(self):
        return self.__points

    @classmethod
    def symnmf(self, k):
        h_matrix, w_matrix = self.init_h(k)
        return symnmfC.symnmf(h_matrix, w_matrix)
    
    @classmethod
    def init_h(self, k):
        w_matrix = self.norm()
        m_avg = np.mean(w_matrix)
        # we initialize the H matrix with random values between 0 and 2*sqrt(m/k)
        h_matrix = (np.random.uniform(0, 1, (len(self.__points), k)) * (2 * math.sqrt(m_avg / k))).tolist()
        return h_matrix, w_matrix

    @classmethod
    def sym(self):
        return symnmfC.sym(self.__points)
    
    @classmethod
    def ddg(self):
        return symnmfC.ddg(self.__points)
    
    @classmethod
    def norm(self):
        return symnmfC.norm(self.__points)
    

    @staticmethod
    # the format is as described in the requirements PDF file
    def print_formatted_matrix(matrix):
        for i in range(len(matrix)):
            for j in range(len(matrix[0]) -1):
                print("%.4f" % matrix[i][j], end=",")
            print("%.4f" % matrix[i][len(matrix[0]) -1])

    @classmethod
    # Derive a hard clustering from the algorithm's result, as described in the PDF
    def parse_to_clusters(self, k):
        h_matrix = self.symnmf(k)
        clusters = []
        for row in h_matrix:
            cluster = row.index(max(row))
            clusters.append(cluster)
        return clusters




def main():
    try:
        # initializing the variables
        k = int(sys.argv[1])
        goal = str(sys.argv[2])
        file_name = str(sys.argv[3])

        proj_symn = SymnmfPython(file_name)

        if (goal == 'symnmf'):
            SymnmfPython.print_formatted_matrix(proj_symn.symnmf(k))
        
        if (goal == 'sym'):
            SymnmfPython.print_formatted_matrix(proj_symn.sym())

        if (goal == 'ddg'):
            SymnmfPython.print_formatted_matrix(proj_symn.ddg())

        if (goal == 'norm'):
            SymnmfPython.print_formatted_matrix(proj_symn.norm())


    except Exception:
        print("An Error Has Occurred")



if __name__ == "__main__":
    main()