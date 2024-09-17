"""
This module implements the K-means clustering algorithm using the following classes:

1. Cluster:
   - Represents a cluster with a list of points and methods to manage them.
   - Provides functionality to add or remove points and update the cluster's mean.

2. Point:
   - Represents a point in the data space with methods to manage its cluster assignment.
   - Provides functionality to find the best cluster and update its assigned cluster.

3. Kmeans:
   - Implements the K-means clustering algorithm.
   - Initializes clusters, assigns points to clusters, and updates cluster means until convergence or max iterations.
   - Provides functionality to execute the algorithm and return cluster assignments.


Usage:
- Create instances of the `Kmeans` class with the desired parameters and call `get_kmeans_result()` to obtain the clustering results.

"""
import numpy as np

class Point:
    """
    Attributes:
    coordinates (numpy.ndarray): The coordinates of the point.
    assigned_cluster (Cluster or None): The cluster to which the point is assigned, or None if not assigned.
    """
    def __init__(self, data):
        self.coordinates = data
        self.assigned_cluster = None
        
    def update_cluster(self, new_cluster):
        if self.assigned_cluster == new_cluster:
            return # There is no need to change
        
        if self.assigned_cluster != None:
            self.assigned_cluster.remove_point(self)
        
        self.assigned_cluster = new_cluster
        self.assigned_cluster.add_point(self)
    
    def find_best_cluster_for_point(self, clusters):
        # Finds the best cluster for the point based on the algorithm's criteria
        min = np.linalg.norm(self.coordinates - clusters[0].get_mean().coordinates)
        min_cluster = clusters[0]
        for cluster in clusters:
            temp = np.linalg.norm(self.coordinates - cluster.get_mean().coordinates)
            if (temp < min):
                min_cluster = cluster
                min = temp
        return min_cluster
    

class Cluster:
    """
    Attributes:
    clusters_points (list of Point): List of points assigned to this cluster.
    prev_mean (Point): The previous mean point of the cluster.
    mean (Point): The current mean point of the cluster.
    id (int): The unique identifier for this cluster.
    """
    def __init__(self, init_point, id):
        self.__clusters_points = [init_point]
        self.__prev_mean = init_point
        self.__mean = init_point
        self.id = id

    def get_prev_mean(self):
        return self.__prev_mean
    
    def get_mean(self):
        return self.__mean
    
    def remove_point(self, point):
        self.__clusters_points.remove(point)

    def add_point(self, point):
        self.__clusters_points.append(point)

    def update_mean(self):
        self.__prev_mean = self.__mean
        coordinates = []
        for point in self.__clusters_points:
            coordinates.append(point.coordinates)
        self.__mean = Point(np.mean(coordinates, axis=0))

                           


class Kmeans:
    """
    Attributes:
    __data (list): The dataset to be clustered, converted to a NumPy array.
    __k (int): The number of clusters to form.
    __max_iter (int): The maximum number of iterations for the K-means algorithm.
    __epsilon (float): The threshold for convergence.
    __clusters_array (list): List to store the clusters.
    __points_array (list): List to store the points.
    """
    def __init__(self, data, k, max_iter, eps):
        self.__data = np.array(data) 
        self.__k = k
        self.__max_iter = max_iter
        self.__epsilon = eps
        self.__clusters_array = []
        self.__points_array = []

    def __kmeans_algo_init(self):
        for point in self.__data:
            self.__points_array.append(Point(point))

        for index in range(self.__k):
            # This initialization does not fully utilize the functions declared above.
            # It requires special handling to directly initialize clusters with the first k points.
            # Alternative designs could indeed be implemented
            new_cluster = Cluster(self.__points_array[index], index)
            self.__clusters_array.append(new_cluster)
            self.__points_array[index].assigned_cluster = new_cluster


    def __all__clusters_converged(self):
        for cluster in self.__clusters_array:
            if np.linalg.norm(cluster.get_mean().coordinates - cluster.get_prev_mean().coordinates) >= self.__epsilon:
                return False
        return True
    

    def __run_kmeans(self):
        self.__kmeans_algo_init()
        iter = 1
        while (iter <= self.__max_iter):
            for point in self.__points_array:
                best_cluster = point.find_best_cluster_for_point(self.__clusters_array)
                point.update_cluster(best_cluster)
            
            for cluster in self.__clusters_array:
                cluster.update_mean()

            iter += 1
            if self.__all__clusters_converged():
                break

    def get_kmeans_result(self):
        self.__run_kmeans()    
        result = []
        for point in self.__points_array:
            result.append(point.assigned_cluster.id)
        return np.array(result)


    def print_clusters(self):
        # Just for checking with HW1, not needed for the project but kept it anyway
        self.__run_kmeans()
        for cluster in self.__clusters_array:
            formatted_mean = ' '.join(f'{coord:.4f}' for coord in cluster.get_mean().coordinates)
            print(formatted_mean)  
