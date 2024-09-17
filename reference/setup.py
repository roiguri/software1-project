"""
This is a standard `setup.py` file used for building and installing the Python
extension module `symnmfC`. It is based on the example provided in the Course 
Lectures. 
The `symnmfC` module wraps the SymNMF algorithm implemented in C and is designed 
to be used as a Python extension.
"""
from setuptools import Extension, setup

module = Extension("symnmfC",
                  sources=[
                    'symnmf.c',
                    'symnmfmodule.c',
                    'matrix.c',
                    'utils.c',
                    'file.c'
                  ])
setup(name='symnmfC',
     version='1.0',
     description='Python wrapper for the symnmf algo C extension',
     ext_modules=[module])