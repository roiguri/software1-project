"""
This script performs clustering analysis using SymNMF and K-means algorithms,
and evaluates clustering performance using silhouette scores.

Note:
- This script is relatively straightforward and primarily uses functionality provided by other
  modules within the project, specifically `symnmf` and `kmeans`.
- It serves as a simple interface to run these algorithms and report their results.

"""
import sys
from sklearn.metrics import silhouette_score
from symnmf import SymnmfPython
from kmeans import Kmeans 
MAX_ITER = 300
EPSILON = 0.0001

def run_symnmf(symn, k):
    return symn.parse_to_clusters(k)

def run_kmeans(data, k, max_iter, eps):
    kmeans_instance = Kmeans(data, k, max_iter, eps)
    return kmeans_instance.get_kmeans_result()


def main():
    try:
        k = int(sys.argv[1])
        file_name = sys.argv[2]

        # Run SymNMF
        symn = SymnmfPython(file_name)
        data = symn.get_data() # Using the already existing functionality defined to parse the data

        # Run SymNMF
        symnmf_clusters = run_symnmf(symn, k)
        symnmf_score = silhouette_score(data, symnmf_clusters)

        # Run Kmeans
        kmeans_clusters = run_kmeans(data, k, MAX_ITER, EPSILON)
        kmeans_score = silhouette_score(data, kmeans_clusters)

        # Print results
        print(f"nmf: {symnmf_score:.4f}")
        print(f"kmeans: {kmeans_score:.4f}")
    
    except Exception:
        print("An Error Has Occurred")


if __name__ == "__main__":
    main()


