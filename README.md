# Design of Algorithms

## Program 1 : Graphs
### Description
In this assignment, you will be implementing a Graph using both an adjacency matrix and an adjacency list. You will use the adjacency matrix to perform a depth first search and the adjacency list to perform a breadth first search. Make sure to account for all connected components.


The required public Graph operations are as follows:
> Graph(int max_num_vertices) //constructor should accept the maximum number of vertices allowed in the graph

> void addVertex(T item) //allow items (items are identified by String search keys) to be stored in the Graph vertices

> void addEdge(String start_vertex_key, String end_vertex_key, double edge_weight) //add a directed edge between two vertices

> List dfs() //perform a depth first search, adding items to a linked list

> List bfs() //perform a breadth first search, adding items to a linked list

> List topological_sort() //compute a topological sort using depth first search, return null if not a dag


The vertices of the Graph will be read in from a text file as in the following file:
> Romanian Cities
> Top Sort Verts
The edges of the Graph will be read in from a text file as in the following file:
> Romanian Mileages
> Top Sort Edges


The program will run by specifying the vertices and edges text files as command line arguments. See the sample output below (the order in which the vertices and edges were added determines the output order, depending on search method):
c:\Program1\Cities.exe romanian_cities.txt romanian_mileages.txt

### Sample Output
```
BFS:

Craiova
Pitesti
Dobreta
Rimnicu_Vilcea
Bucharest
Mehadia
Sibiu
Fagaras
Giurgiu
Urziceni
Lugoj
Oradea
Arad
Hirsova
Vaslui
Timisoara
Zerind
Eforie
Iasi
Nearnt



DFS:

Craiova
Pitesti
Bucharest
Fagaras
Sibiu
Arad
Zerind
Oradea
Timisoara
Lugoj
Mehadia
Dobreta
Rimnicu_Vilcea
Giurgiu
Urziceni
Hirsova
Eforie
Vaslui
Iasi
Nearnt



BFS: (Directed)

0
4
2
3
8
6
7
9
10
1
5


BFS: (Undirected)

0
4
2
3
8
6
7
1
9
10
5


DFS: (Directed)

0
2
6
7
9
10
5
1
3
8
4


DFS: (Undirected)

0
2
6
7
8
9
10
4
5
1
3


TOPOLOGICAL SORT: (Directed)

1
5
0
4
3
2
8
7
10
9
6


TOPOLOGICAL SORT: (Undirected)

0
2
8
4
7
10
5
1
3
9
6
```

## Program 2 : Minimum Spanning Tree

### Description
In this assignment, you will be finding the minimum spanning tree of a graph in two ways.
> Prim's Algorithm 

>Kruskal's Algorithm

For Kruskal's use the quick union method as discussed in class.You will also implement Dijkstra's shortest path algorithm using the efficient technique (using decreaseKey) discussed in class.

Implement a Graph ADT that stores the edges in an adjacency list. Your graph should have the following capabilities:
```
void addVertex(T item) //adds a Vertex to the Graph with a unique String identifier stored at the Vertex

void addEdge(String start_vertex_key, String end_vertex_key, double edge_weight) //adds a directed Edge between the two specified vertices, with the specified weight

List prim() //returns a List of the edges in the minimum spanning tree computed using Prim's algorithm (use the first vertex in the vertex list as the starting vertex)

List kruskal() //returns a List of the edges in the minimum spanning tree computed using Kruskal's algorithm

List shortestPath(String start_vertex_key, String end_vertex_key) //returns the shortest path between the specified vertices
```
Read in the text file
> US Cities

which contains a list of major U.S. cities and store the city names in the vertices of your Graph.

Read in the text file
> Cities Milage

which contains the mileage between pairs of cities and create edges in your Graph with the mileage stored as the weight of an edge.

Allow the input files to be specified as command line arguments. Thus, allow the user to type

> MinSpan vertex_file.txt edge_file.txt vertex_start_key vertex_end_key

at the command prompt to execute your program.

### Sample Output
```
PRIM
(Denver, Dallas, 663)
(Dallas, Birmingham, 581)
(Birmingham, Chicago, 578)
(Chicago, Cleveland, 308)
(Cleveland, Buffalo, 173)
(Buffalo, Boston, 400)


KRUSKAL
(Buffalo, Cleveland, 173)
(Chicago, Cleveland, 308)
(Boston, Buffalo, 400)
(Birmingham, Chicago, 578)
(Birmingham, Dallas, 581)
(Dallas, Denver, 663)


DIJKSTRA
Travel from Hirsova to Pitesti
Total Cost : 284 (Hirsova, Urziceni, 98)
(Urziceni, Bucharest, 183)
(Bucharest, Pitesti, 284)
```

