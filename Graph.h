/***
 *
 *     BY: RAYVIN CARL AMADOR
 *
 *     TO COMPILE THIS PROGRAM:
 *        1.) g++ GraphDriver.cpp -o MinSpan
 *        2.) ./MinSpan vertex_file.txt edge_file.txt
 *        3.) ./MinSpan romanian_cities.txt romanian_mileages.txt Hirsova Pitesti
 *
 */

#if !defined GRAPH_H
#define GRAPH_H

#include "DataStructures/ListLinked.h"
#include "DataStructures/City.h"
#include "DataStructures/StackArray.h"
#include "DataStructures/QueueArray.h"
#include "DataStructures/CityToCityConnection.h"
#include "DataStructures/SortedArray.h"
#include "DataStructures/QuickUnion.h" // ACTS LIKE A TREE. USED TO DETERMINE CONNECTIONS FOR KRUSKAL'S ALGORITHM

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <climits>
using namespace std;

template <class T>
class Graph
{
private:
	int max_vertices;
	T *array_of_vertex_city_objects;					 // ARRAY USED TO HOLD VERTICES
	int current_size_of_array;							 // SIZE OF array_of_vertex_city_objects ARRAY
	int **adjacency_matrix;								 // 2 DIMENSIONAL ARRAY
	ListLinked<T> **adjacency_list;						 // ARRAY OF LINKED LISTS
	int counter_for_visits;								 // USED IN BFS AND DFS RECURSIVE PARTS
	bool has_adjacency_list_and_matrix_been_created_yet; // START OUT AS FALSE

	ListLinked<T> *linked_list_dfs_objects;		 // FINAL RESULT OF DFS ITEMS VISITED IN ORDER
	StackArray<T> *stack_for_dfs;				 // ITEMS ADDED ON THIS AND THEN POPPED OFF
	StackArray<T> *stack_for_topological_sort;	 // POP (STARTING FROM BACK) TO GET TOPOLOGICAL ORDER
	ListLinked<T> *topological_sort_linked_list; // FINAL RESULT OF TOPOLOGICAL SORT
	ListLinked<T> *linked_list_bfs_objects;		 // FINAL RESULT OF BFS ITEMS VISITED IN ORDER
	QueueArray<T> *queue_for_bfs;				 // ITEMS ADDED ON THIS AND THEN DEQUEUED OFF
	bool *rows_visited_bfs;						 // KEEPS TRACK OF ROWS VISITED IN BFS

	bool *visited_cities_in_prims;
	bool *visited_cities_in_kruskals;

	SortedArray *sorted_array_prims;
	ListLinked<CityToCityConnection> *prims_result; // HOLDS THE ORDER OF CONNECTIONS FOR PRIM'S ALGORITHM
	SortedArray *sorted_array_kruskals;
	ListLinked<CityToCityConnection> *kruskals_result; // HOLDS THE ORDER OF CONNECTIONS FOR KRUSKAL'S ALGORITHM
	QuickUnion *quick_union_pointer;
	SortedArray *sorted_array_shortest_path;
	ListLinked<CityToCityConnection> *shortest_path_result;

	// PERFORMS A RECURISIVE DEPTH FIRST SEARCH
	void dfs_recursive(int row);

	// PERFORMS A RECURISIVE BREADTH FIRST SEARCH
	void bfs_recursive(int row);

	// DYNAMICALLY RESIZES ARRAY
	void resizeArray(int new_max_vertices);

public:
	// CONSTRUCTOR TAKES IN THE MAXIMUM NUMBER OF VERTICES
	// ALLOWED IN THE GRAPH. STARTS WITH 2 AND DYNAMICALLY RESIZES
	Graph(int max_num_vertices);
	~Graph();

	// READS FROM A VERTICES FILE AND ADDS ITEMS TO A REGULAR
	// 1D ARRAY IN THE SAME ORDER AS IT WAS IN THE TEXT FILE
	void addVertex(T item);

	// MUST CALL createAdjacencyMatrixAndAdjacencyList() BEFORE
	// DOING THIS. READS FROM AN EDGE FILE AND ADDS THE EDGES TO
	// BOTH THE ADJACENCY MATRIX AND ADJACENCY LIST
	// THIS MAKES EVERYTHING UNDIRECTED, BUT TO MAKE IT WORK WITH
	// BOTH DIRECTED AND UNDIRECTED, COMMENT THIS FUNCTION OUT
	// AND UNCOMMENT THE OTHER VERSION OF addEdge BELOW THIS.
	void addEdge(string start_vertex_key, string end_vertex_key, double edge_weight);

	// CALLS dfs_recursive()
	ListLinked<T> dfs();

	// CALLS bfs_recursive()
	ListLinked<T> bfs();

	// THIS RETURNS THE REVERSE OF THE POP ORDER FROM dfs().
	// RETURNS NULL IF NOT A DIRECT ACYCLIC GRAPH (DAG)
	ListLinked<T> topological_sort();

	// AFTER DOING BOTH addVertex() AND addEdge(), THIS
	// DISPLAYS A VISUAL OF WHAT THE SQUARE MATRIX LOOKS LIKE
	void displayMatrix();

	// AFTER DOING BOTH addVertex AND addEdge, THIS DISPLAYS
	// A VISUAL OF WHAT THE ARRAY OF LINKED LISTS LOOKS LIKE
	void displayAdjacencyList();

	//*********************
	// PROGRAM 2 STUFF
	//*********************
	// RETURNS A LIST OF THE EDGES IN THE MINIMUM SPANNING TREE
	// COMPUTED USING PRIM'S ALGORITHM (USE THE FIRST VERTEX IN
	// THE VERTEX LIST AS THE STARTING VERTEX)
	ListLinked<CityToCityConnection> prim();

	// RETURNS A LIST OF THE EDGES IN THE MINIMUM SPANNING TREE
	// COMPUTED USING KRUSKSAL'S ALGORITHM
	ListLinked<CityToCityConnection> kruskal();

	// RETURNS THE SHORTEST PATH BETWEEN THE SPECIFIED VERTICES
	ListLinked<CityToCityConnection> shortestPath(string start_vertex_key, string end_vertex_key);

	void displayAdjacencyListWeights();
};

//*************
// CONSTRUCTOR
//*************
template <class T>
Graph<T>::Graph(int max_num_vertices)
{
	current_size_of_array = 0;
	counter_for_visits = 0;
	max_vertices = max_num_vertices;
	linked_list_dfs_objects = new ListLinked<T>();
	stack_for_dfs = new StackArray<T>();
	stack_for_topological_sort = new StackArray<T>();
	topological_sort_linked_list = new ListLinked<T>();
	linked_list_bfs_objects = new ListLinked<T>();
	queue_for_bfs = new QueueArray<T>();
	has_adjacency_list_and_matrix_been_created_yet = false;

	//-----------------------------------------
	// INITIALIZE ARRAY TO HOLD LIST OF VERTEX
	//-----------------------------------------
	array_of_vertex_city_objects = new T[max_num_vertices];

	prims_result = new ListLinked<CityToCityConnection>();
	sorted_array_prims = new SortedArray();
	kruskals_result = new ListLinked<CityToCityConnection>();
	sorted_array_kruskals = new SortedArray();
	shortest_path_result = new ListLinked<CityToCityConnection>();
	sorted_array_shortest_path = new SortedArray();
}

//*************
// DESTRUCTOR
//*************
template <class T>
Graph<T>::~Graph()
{
	// cout << "\n\nDEALLOCATING NOW...\n\n";
	delete[] array_of_vertex_city_objects;
	delete[] adjacency_matrix; // ** DOUBLE CHECK
	delete[] adjacency_list;   // ** DOUBLE CHECK
	delete linked_list_dfs_objects;
	delete stack_for_dfs;
	delete stack_for_topological_sort;
	delete topological_sort_linked_list;
	delete linked_list_bfs_objects;
	delete queue_for_bfs;
	delete[] rows_visited_bfs;
	// cout << "DONE DEALLOCATING EVERYTHING\n\n";
}

template <class T>
void Graph<T>::addVertex(T item) // ITEM IS A POINTER TO A CITY OBJECT
{
	//-----------------------------------------
	// RESIZE (DOUBLE) THE ARRAY IF IT IS FULL
	//-----------------------------------------
	if (current_size_of_array == max_vertices)
	{
		resizeArray(2 * max_vertices);
	}

	//-----------------------------------------------------------------------
	// ADD THE ITEM THAT WAS READ FROM THE TEXTFILE TO THE ARRAY OF VERTICES
	//-----------------------------------------------------------------------
	array_of_vertex_city_objects[current_size_of_array] = item;

	//-------------------------
	// INCREMENT SIZE OF ARRAY
	//-------------------------
	current_size_of_array++;

	//---------------------------------------
	// COUT CHECK. COMMENT THIS OUT IN FINAL
	//---------------------------------------
	string city_name = item.getCityName();
	cout << left;
	cout << "After adding " << setw(4) << "--- " << setw(15) << city_name << "  Array Size: " << setw(3) << current_size_of_array << setw(3) << " "
		 << "Max Vertices: " << setw(3) << max_vertices << endl;
}

template <class T>
void Graph<T>::resizeArray(int new_max_vertices)
{
	//-----------------------------------
	// CHANGE PRIVATE VARIABLE'S NEW MAX
	//-----------------------------------
	max_vertices = new_max_vertices;

	//-----------------------------------------------------
	// MAKE A TEMPORARY ARRAY THAT WILL HOLD CLASS OBJECTS
	//-----------------------------------------------------
	T *temp_array = new T[max_vertices];

	//----------------------------------------------------------------------
	// TRANSFER ITEMS FROM FULL ARRAY TO -> THE NEWLY CREATED, BIGGER ARRAY
	//----------------------------------------------------------------------
	for (int i = 0; i < current_size_of_array; i++)
	{
		temp_array[i] = array_of_vertex_city_objects[i];
	}

	delete[] array_of_vertex_city_objects;
	array_of_vertex_city_objects = temp_array;
}

/*********************************************************************************
 *
 * THIS addEdge FUNCTION WORKS FOR BOTH DIRECTED AND UNDIRECTED EDGES
 *
 ********************************************************************************/
template <class T>
void Graph<T>::addEdge(string start_vertex_key, string end_vertex_key, double edge_weight)
{
	//------------------------------------------------
	// INITIALIZE ADJACENCY MATRIX AND ADJACENCY LIST.
	// ONLY DO THIS ONCE, THIS IS WHY I HAVE A BOOL
	// VARIABLE THAT GETS SET TO TRUE AFTER THIS RUNS

	// FOR THE FIRST TIME
	//------------------------------------------------
	if (has_adjacency_list_and_matrix_been_created_yet == false)
	{
		// INITIALIZE 2-D ARRAY FOR ADJACENCY MATRIX AND INITIALIZE ALL WITH 0
		adjacency_matrix = new int *[current_size_of_array]; // current_size_of_array = ROWS
		for (int i = 0; i < current_size_of_array; i++)
		{
			adjacency_matrix[i] = new int[current_size_of_array]; // current_size_of_array = COLUMNS
		}

		// INITIALIZE ALL AREAS OF THE MATRIX WITH 0
		for (int row = 0; row < current_size_of_array; row++)
		{
			for (int column = 0; column < current_size_of_array; column++)
			{
				adjacency_matrix[row][column] = 0;
			}
		}

		// INITIALIZE ARRAY OF LINKED LISTS FOR ADJACENCY LIST
		adjacency_list = new ListLinked<T> *[current_size_of_array];
		for (int i = 0; i < current_size_of_array; i++)
		{
			adjacency_list[i] = new ListLinked<T>();

			// SET THE FIRST ITEM IN EACH SPOT TO BE THE SAME AS THE LIST OF ROMANIAN CITIES
			// adjacency_list[i]->add(&array_of_vertex_city_objects[i]);

			// CREATE A NEW OBJECT TO STORE IN THE ARRAY OF LINKED LISTS
			T *new_city_object = new T();

			// GET THE NAME OF THE CITY THAT NEEDS TO BE ADDED TO THE LINKED LIST
			string city_name_string = array_of_vertex_city_objects[i].getCityName();

			// PUT THAT SAME STRING NAME IN THE NEW OBJECT
			new_city_object->addCity(city_name_string);

			// ADD THE WEIGHT TO THE NEW CITY OBJECT
			// new_city_object->addWeight(edge_weight);

			// ADD THE NEW OBJECT WITH THE SAME EXACT NAME TO THE ARRAY OF LINKED LISTS
			adjacency_list[i]->add(new_city_object);
		}

		// SET TO TRUE SO IT DOES NOT REPEAT AGAIN
		has_adjacency_list_and_matrix_been_created_yet = true;
	}

	//--------------------------------------------------
	// FIND WHICH ONE start_vertex_key MATCHES WITH IN
	// array_of_vertex_city_objects
	//--------------------------------------------------
	int index_of_start_vertex;
	for (int i = 0; i < current_size_of_array; i++)
	{
		if (start_vertex_key == array_of_vertex_city_objects[i].getCityName())
		{
			index_of_start_vertex = i;
		}
	}

	//-----------------------------------------------
	// FIND WHICH ONE end_vertex_key MATCHES WITH IN
	// array_of_vertex_city_objects
	//-----------------------------------------------
	int index_of_end_vertex;
	for (int i = 0; i < current_size_of_array; i++)
	{
		if (end_vertex_key == array_of_vertex_city_objects[i].getCityName())
		{
			index_of_end_vertex = i;
		}
	}

	//--------------------------------------------------------------
	// RENAME VARIABLES TO MAKE THINGS CLEARER FOR ADJACENCY MATRIX
	//--------------------------------------------------------------
	int row = index_of_start_vertex;
	int column = index_of_end_vertex;

	//------------------------------
	// ADD EDGE TO ADJACENCY MATRIX
	//------------------------------
	adjacency_matrix[row][column] = edge_weight;

	//------------------------------------------------------------
	// RENAME VARIABLES TO MAKE THINGS CLEARER FOR ADJACENCY LIST
	//------------------------------------------------------------
	int array_index = row;
	int city_object = column;

	//---------------------------------------------------------------------------
	// ADD EDGE TO ADJACENCY LIST.
	// THIS IS ALL I NEED TO MAKE THIS PROGRAM WORK BOTH DIRECTED AND UNDIRECTED
	//---------------------------------------------------------------------------
	// adjacency_list[array_index]->add(&array_of_vertex_city_objects[city_object]);

	//---------------------------------------------
	// CHECK ROW OF START VERTEX KEY BEFORE ADDING
	// EDGE TO ADJACENCY LIST.
	// THIS IS TO MAKE SURE THE ITEM DOES NOT GET
	// PUT IN THE SAME INDEX TWICE
	//---------------------------------------------
	ListLinkedIterator<T> *iter = adjacency_list[array_index]->iterator();
	int size_of_LL = adjacency_list[array_index]->size();
	bool already_there = false;
	StackArray<T> *temp_stack = new StackArray<T>();
	while (iter->hasNext())
	{
		T *item = iter->next();
		temp_stack->push(item);
	}

	for (int x = 0; x < size_of_LL; x++)
	{
		T *item = temp_stack->pop();

		// COMPARES CITY NAMES
		if (item->getCityName() == array_of_vertex_city_objects[city_object].getCityName())
		{
			already_there = true;
		}
	}

	if (already_there == false)
	{
		// CREATE A NEW OBJECT TO STORE IN THE ARRAY OF LINKED LISTS
		T *new_city_object = new T();

		// GET THE NAME OF THE CITY THAT NEEDS TO BE ADDED TO THE LINKED LIST
		string city_name_string = array_of_vertex_city_objects[city_object].getCityName();

		// PUT THAT SAME STRING NAME IN THE NEW OBJECT
		new_city_object->addCity(city_name_string);

		// ADD THE WEIGHT TO THE NEW CITY OBJECT
		new_city_object->addWeight(edge_weight);

		// ADD THE NEW OBJECT WITH THE SAME EXACT NAME TO THE ARRAY OF LINKED LISTS
		adjacency_list[array_index]->add(new_city_object);
		// int weight1 = adjacency_list[array_index]->get(1)->getWeight();
		// int weight1 = new_city_object->getWe(0);
		// cout << "\nWEIGHT IS: " << weight1;
	}
}

/*********************************************************************************
 *
 * THIS addEdge FUNCTION WORKS FOR UNDIRECTED EDGES ONLY TO WORK WITH
 * DR. BROWN'S INSTRUCTIONS
 *
 ********************************************************************************/
/*
template < class T >
void Graph<T>::addEdge(string start_vertex_key, string end_vertex_key, double edge_weight)
{
	//------------------------------------------------
	// INITIALIZE ADJACENCY MATRIX AND ADJACENCY LIST.
	// ONLY DO THIS ONCE, THIS IS WHY I HAVE A BOOL
	// VARIABLE THAT GETS SET TO TRUE AFTER THIS RUNS
	// FOR THE FIRST TIME
	//------------------------------------------------
	if (has_adjacency_list_and_matrix_been_created_yet == false)
	{
		// INITIALIZE 2-D ARRAY FOR ADJACENCY MATRIX AND INITIALIZE ALL WITH 0
		adjacency_matrix = new int*[current_size_of_array]; // current_size_of_array = ROWS
		for (int i = 0; i < current_size_of_array; i++)
		{
			adjacency_matrix[i] = new int[current_size_of_array]; // current_size_of_array = COLUMNS
		}

		// INITIALIZE ALL AREAS OF THE MATRIX WITH 0
		for (int row = 0; row < current_size_of_array; row++)
		{
			for (int column = 0; column < current_size_of_array; column++)
			{
				adjacency_matrix[row][column] = 0;
			}
		}

		// INITIALIZE ARRAY OF LINKED LISTS FOR ADJACENCY LIST
		adjacency_list = new ListLinked<T>*[current_size_of_array];
		for (int i = 0; i < current_size_of_array; i++)
		{
			adjacency_list[i] = new ListLinked<T>();

			// SET THE FIRST ITEM IN EACH SPOT TO BE THE SAME AS THE LIST OF ROMANIAN CITIES
			adjacency_list[i]->add(&array_of_vertex_city_objects[i]);
		}

		// SET TO TRUE SO IT DOES NOT REPEAT AGAIN
		has_adjacency_list_and_matrix_been_created_yet = true;
	}

	//--------------------------------------------------
	// FIND WHICH ONE start_vertex_key MATCHES WITH IN
	// array_of_vertex_city_objects
	//--------------------------------------------------
	int index_of_start_vertex; // VARIABLE DEFINITION
	for (int i = 0; i < current_size_of_array; i++)
	{
		if (start_vertex_key == array_of_vertex_city_objects[i].getCityName())
		{
			index_of_start_vertex = i;
		}
	}

	//-----------------------------------------------
	// FIND WHICH ONE end_vertex_key MATCHES WITH IN
	// array_of_vertex_city_objects
	//-----------------------------------------------
	int index_of_end_vertex; // VARIABLE DEFINITION
	for (int i = 0; i < current_size_of_array; i++)
	{
		if (end_vertex_key == array_of_vertex_city_objects[i].getCityName())
		{
			index_of_end_vertex = i;
		}
	}

	//--------------------------------------------------------------
	// RENAME VARIABLES TO MAKE THINGS CLEARER FOR ADJACENCY MATRIX
	//--------------------------------------------------------------
	int row = index_of_start_vertex;
	int column = index_of_end_vertex;

	//------------------------------
	// ADD EDGE TO ADJACENCY MATRIX
	//------------------------------
	adjacency_matrix[row][column] = edge_weight;

	//---------------------------------------------------------------------------
	// (FOR UNDIRECTED ADJACENCY MATRIX)
	// THIS LINE OF CODE IS SO THAT THE PROGRAM MAKES EVERYTHING UNDIRECTED.
	// IT SWAPS THE START & END AND ADDS TO THE OTHER SIDE OF THE MATRIX
	// MAKING IT SYMMETRICAL.
	//---------------------------------------------------------------------------
	adjacency_matrix[column][row] = edge_weight;

	//------------------------------------------------------------
	// RENAME VARIABLES TO MAKE THINGS CLEARER FOR ADJACENCY LIST
	//------------------------------------------------------------
	int array_index = row;
	int city_object = column;

	//---------------------------------------------------------------------------
	// (FOR UNDIRECTED ADJACENCY MATRIX)
	// THIS LINE OF CODE IS SO THAT THE PROGRAM MAKES EVERYTHING UNDIRECTED.
	// IT SWAPS THE START & END AND ADDS TO THE OTHER ARRAY INDEX IN THE
	// ADJACENCY LIST
	//---------------------------------------------------------------------------
	//---------------------------------------------
	// CHECK ROW OF START VERTEX KEY BEFORE ADDING
	//---------------------------------------------
	ListLinkedIterator<T>* iter = adjacency_list[array_index]->iterator();
	int size_of_LL = adjacency_list[array_index]->size();
	bool already_there = false;
	StackArray<T> *temp_stack = new StackArray<T>();
	while (iter->hasNext())
	{
		T *item = iter->next();
		temp_stack->push(item);
	}

	for (int x = 0; x < size_of_LL; x++)
	{
		T *item = temp_stack->pop();

		// COMPARES CITY NAMES
		if (item->getCityName() == array_of_vertex_city_objects[city_object].getCityName())
		{
			already_there = true;
		}
	}

	if (already_there == false)
	{
		adjacency_list[array_index]->add(&array_of_vertex_city_objects[city_object]);
	}

	//---------------------------------------------
	// CHECK ROW OF END VERTEX KEY BEFORE ADDING
	//---------------------------------------------
	ListLinkedIterator<T>* iter2 = adjacency_list[city_object]->iterator();
	int size_of_LL2 = adjacency_list[city_object]->size();
	bool already_there2 = false;
	StackArray<T> *temp_stack2 = new StackArray<T>();
	while (iter2->hasNext())
	{
		T *item = iter2->next();
		temp_stack2->push(item);
	}

	for (int y = 0; y < size_of_LL2; y++)
	{
		T *item = temp_stack2->pop();

		// COMPARES CITY NAMES
		if (item->getCityName() == array_of_vertex_city_objects[array_index].getCityName())
		{
			already_there2 = true;
		}
	}

	if (already_there2 == false)
	{
		adjacency_list[city_object]->add(&array_of_vertex_city_objects[array_index]);
	}
}
*/

template <class T>
void Graph<T>::displayMatrix()
{
	cout << left << endl
		 << endl;
	cout << "--------------------------------------------------------------------------------\n";
	cout << "                      THIS IS WHAT THE MATRIX LOOKS LIKE: \n";
	cout << "--------------------------------------------------------------------------------\n";
	int counter1 = 0;
	for (int row = 0; row < current_size_of_array; row++)
	{
		for (int column = 0; column < current_size_of_array; column++)
		{
			// cout << setprecision(3);
			cout << setw(4) << adjacency_matrix[row][column] << setw(1) << " ";
			counter1++;

			if (counter1 == current_size_of_array)
			{
				cout << endl
					 << endl;
				counter1 = 0;
			}
		}
	}
}

template <class T>
void Graph<T>::displayAdjacencyList()
{
	//---------------------------------------------------------------
	// DISPLAY WHAT THE ADJACENCY LIST LOOKS LIKE (SAME AS ON PAPER)
	//---------------------------------------------------------------
	cout << endl
		 << endl;
	cout << "--------------------------------------------------------------------------------\n";
	cout << "              THIS IS WHAT THE ADJACENCY LIST LOOKS LIKE ON PAPER: " << endl;
	cout << "--------------------------------------------------------------------------------\n";
	for (int j = 0; j < current_size_of_array; j++)
	{
		ListLinkedIterator<T> *iter = adjacency_list[j]->iterator();
		while (iter->hasNext())
		{
			T *city_object = iter->next();
			string city_name = city_object->getCityName();
			int city_weight = city_object->getWeight();
			cout << city_weight << " " << city_name;

			// THIS IS SO IT DOESN'T ADD ANOTHER -----> AT THE END IF THERE IS NOTHING ELSE THERE
			if (iter->hasNext())
			{
				cout << " ---> ";
			}
		}
		cout << endl
			 << endl;
	}
}

template <class T>
ListLinked<T> Graph<T>::dfs()
{
	//------------------------------------------------------
	// USE THE 1-D ARRAY THAT HOLDS THE CITY OBJECTS.
	// MAKE EVERY VISITED VARIABLE OF THE CITY OBJECTS = 0
	//------------------------------------------------------
	for (int x = 0; x < current_size_of_array; x++)
	{
		array_of_vertex_city_objects[x].changeVisitedStatus(0);
	}

	//----------------------------------------------
	// MAKE SURE THE counter_for_visits STARTS AT 0
	//----------------------------------------------
	counter_for_visits = 0;

	//---------------------------------------------
	// GO THROUGH THE ENTIRE ROWS OF THE 2-D ARRAY
	//---------------------------------------------
	for (int i = 0; i < current_size_of_array; i++)
	{
		if (array_of_vertex_city_objects[i].getVisitedStatus() <= 0)
		{
			dfs_recursive(i);
		}
	}

	//---------------------------------------------------------------------------------
	// SORT THE OBJECTS IN THE ARRAY TO BE IN ASCENDING ORDER. Object with visited = 1
	// MEANS IT WAS THE FIRST ITEM VISITED
	//
	// THIS GOES THROUGH 1 TO SIZE OF ARRAY
	//---------------------------------------------------------------------------------
	for (int r = 1; r <= current_size_of_array; r++)
	{
		for (int s = 0; s < current_size_of_array; s++)
		{
			if (array_of_vertex_city_objects[s].getVisitedStatus() == r)
			{
				linked_list_dfs_objects->add(&array_of_vertex_city_objects[s]);
			}
		}
	}

	return *linked_list_dfs_objects;
}

template <class T>
void Graph<T>::dfs_recursive(int row) // i IS THE ROW
{
	//--------------------------------------------------------------
	// INCREMENT SO THAT THE ITEMS HAVE CONSECUTIVE VISITED NUMBERS
	//--------------------------------------------------------------
	counter_for_visits++;

	//--------------
	// ADD TO STACK
	//--------------
	stack_for_dfs->push(&array_of_vertex_city_objects[row]);

	//---------------------
	// CHANGE VISIT STATUS
	//---------------------
	array_of_vertex_city_objects[row].changeVisitedStatus(counter_for_visits);

	//------------------------------
	// GO THROUGH THE ROW'S COLUMNS
	//------------------------------
	for (int a = 0; a < current_size_of_array; a++)
	{
		// IF THERE IS A WEIGHT AND IF IT HAS NOT BEEN VISITED YET, DO THIS
		if (adjacency_matrix[row][a] != 0 && array_of_vertex_city_objects[a].getVisitedStatus() <= 0)
		{
			dfs_recursive(a);
		}
	}

	//---------------------------------------------------------------
	// POP FROM stack_for_dfs AND PUSH TO stack_for_topological_sort
	//---------------------------------------------------------------
	T *item = stack_for_dfs->pop();
	stack_for_topological_sort->push(item);
}

template <class T>
ListLinked<T> Graph<T>::bfs()
{
	//-----------------------------------------------------------
	// INITIALIZE THE BOOL PRIVATE ARRAY AND MARK ALL WITH FALSE
	//-----------------------------------------------------------
	rows_visited_bfs = new bool[current_size_of_array];
	for (int a = 0; a < current_size_of_array; a++)
	{
		rows_visited_bfs[a] = false;
	}

	//------------------------------------------------------------
	// MAKE THE COUNTER START AT 0 AGAIN SINCE IT WAS USED IN DFS
	// AND IT MIGHT NOT BE 0 ANYMORE
	//------------------------------------------------------------
	counter_for_visits = 0;

	//------------------------------------------------------
	// USE THE 1-D ARRAY THAT HOLDS THE CITY OBJECTS.
	// MAKE EVERY VISITED VARIABLE OF THE CITY OBJECTS = 0
	//------------------------------------------------------
	for (int x = 0; x < current_size_of_array; x++)
	{
		array_of_vertex_city_objects[x].changeVisitedStatus(0);
	}

	//-----------------------------------------------------
	// GO THROUGH ALL INDICES OF THE ARRAY OF LINKED LISTS
	//-----------------------------------------------------
	for (int array_index = 0; array_index < current_size_of_array; array_index++)
	{
		if (array_of_vertex_city_objects[array_index].getVisitedStatus() <= 0)
		{
			bfs_recursive(array_index);
		}
	}

	return *linked_list_bfs_objects;
}

template <class T>
void Graph<T>::bfs_recursive(int row)
{
	//----------------------------------
	// GO THROUGH THE ROW'S LINKED LIST
	//----------------------------------
	ListLinkedIterator<T> *iter = adjacency_list[row]->iterator();
	while (iter->hasNext())
	{
		T *city_object = iter->next(); // I DON'T KNOW THE INDEX OF THIS IN THE ARRAY OF LINKED LISTS YET
		int index_key;

		//----------------------------------------------------------------------------------------
		// FIND WHICH INDEX THIS OBJECT (city_object) BELONGS IN THE array_of_vertex_city_objects
		// TO KNOW WHICH ONE TO CHECK FOR VISITED
		//----------------------------------------------------------------------------------------
		for (int j = 0; j < current_size_of_array; j++)
		{
			// IF THEY EQUAL TO EACH OTHER, SET THE INDEX_KEY TO THAT
			if (city_object->getCityName() == array_of_vertex_city_objects[j].getCityName())
			{
				index_key = j;
			}
		}

		if (city_object->getVisitedStatus() <= 0)
		{
			// ENQUEUE IN QUEUE
			queue_for_bfs->enqueue(&array_of_vertex_city_objects[index_key]);

			// MARK WHEN IT WAS VISITED (CONSECUTIVELY)
			counter_for_visits++;
			array_of_vertex_city_objects[index_key].changeVisitedStatus(counter_for_visits);

			// ADD ITEM TO FINAL LINKED LIST THAT GETS OUTPUTTED IN ASCENDING ORDER
			// CORRESPONDING TO THEIR VISITED STATUS
			linked_list_bfs_objects->add(&array_of_vertex_city_objects[index_key]);
		}
	}

	//-------------------------------------------------------
	// MARK THE ENTIRE ROW AS VISITED IN THE ARRAY THAT ONLY
	// KEEPS TRACK OF A BOOLEAN
	//-------------------------------------------------------
	rows_visited_bfs[row] = true;

	//-----------------------------------------------------
	// AFTER GOING THROW THE ENTIRE ROW DEQUEUE FROM QUEUE
	//-----------------------------------------------------
	queue_for_bfs->dequeue();

	//-----------------------------------------------------------
	// CHECK IF THE QUEUE THAT HOLDS THE ITEMS IS EMPTY. IF SO,
	// GO BACK TO dfs() AND GO TO THE NEXT ROW THAT HAS NOT BEEN
	// VISITED YET
	//-----------------------------------------------------------
	if (queue_for_bfs->isEmpty() == false)
	{
		// PEEK AT THE NEXT ROW TO CHECK
		T *city_object = queue_for_bfs->peek();

		// CHECK WHICH INDEX THE OBJECT IS
		int index_key2;
		for (int j = 0; j < current_size_of_array; j++)
		{
			// COMPARE WITH ARRAY OF VERTICES. IF THEY EQUAL TO EACH OTHER, SET THE INDEX_KEY TO THAT
			if (city_object->getCityName() == array_of_vertex_city_objects[j].getCityName())
			{
				index_key2 = j;
			}
		}

		// CHECK IF IT WAS VISITED ALREADY. IF NOT, GO TO THAT ROW AND RECURSE
		if (rows_visited_bfs[index_key2] == false)
		{
			bfs_recursive(index_key2);
		}
	}
}

template <class T>
ListLinked<T> Graph<T>::topological_sort()
{
	//---------------------------------------------
	// POP ITEMS FROM STACK AND ADD TO LINKED LIST
	//---------------------------------------------
	while (!stack_for_topological_sort->isEmpty())
	{
		// POP FROM STACK
		T *item = stack_for_topological_sort->pop();

		// ADD TO LINKED LIST
		topological_sort_linked_list->add(item);
	}

	return *topological_sort_linked_list;
}

//*********************
// PROGRAM 2 STUFF
//*********************

template <class T>
void Graph<T>::displayAdjacencyListWeights()
{
	//---------------------------------------------------------------
	// DISPLAY HOW THE ADJACENCY LIST TRANSLATES TO IN WORDS
	//---------------------------------------------------------------
	cout << endl
		 << endl;
	cout << "--------------------------------------------------------------------------------\n";
	cout << "              THIS IS HOW THE ADJACENCY LIST TRANSLATES TO IN WORDS: " << endl;
	cout << "--------------------------------------------------------------------------------";
	for (int i = 0; i < current_size_of_array; i++)
	{
		cout << endl;
		// GET THE NAME OF THE FIRST NODE OF THE INDEX i
		T *city_object = adjacency_list[i]->get(1);
		string first_node = city_object->getCityName();

		ListLinkedIterator<T> *iter = adjacency_list[i]->iterator();
		// PULL OUT THE FIRST ONE SO IT DOES NOT SAY IT CONNECTS TO ITSELF
		iter->next();
		while (iter->hasNext())
		{
			T *city_object = iter->next();
			string end_node = city_object->getCityName();
			int city_weight = city_object->getWeight();

			cout << first_node << " connects to " << end_node << " and the weight is: " << city_weight << endl;
		}
		// ListLinkedIterator<T>* iter23 = adjacency_list[i]->iterator();

		// T* city_object = adjacency_list[i]->get(2);
		// T *city_object = iter23->next();
		// string string_name = city_object->getCityName();
		// int weight = city_object->getWeight();

		// cout << "\nFirst node in index " << setw(2) << i << " is: " << setw(15) << string_name << " with weight: " << weight << endl;
		/*
		ListLinkedIterator<T>* iter = adjacency_list[i]->iterator();
		while(iter->hasNext())
		{
			T* city_object = iter->next();

		}*/
	}
}

template <class T>
ListLinked<CityToCityConnection> Graph<T>::prim()
{
	//-------------------------------------------------------------------
	// INITIALIZE ARRAY OF BOOLEANS. USE TO KEEP TRACK OF VISITED CITIES
	//-------------------------------------------------------------------
	visited_cities_in_prims = new bool[current_size_of_array];
	for (int a = 0; a < current_size_of_array; a++)
	{
		visited_cities_in_prims[a] = false;
	}

	//-------------------------------------------------------------
	// START WITH THE FIRST CITY (SAME ORDER AS THE LIST PROVIDED)
	//-------------------------------------------------------------
	T *city_object = adjacency_list[0]->get(1);
	string start_city_name = city_object->getCityName();
	ListLinkedIterator<T> *iter = adjacency_list[0]->iterator();

	// PULL OUT THE FIRST ONE SO IT DOES NOT SAY IT CONNECTS TO ITSELF
	iter->next();

	// GO THROUGH THE ADJACENCY LIST'S ROW
	while (iter->hasNext())
	{
		T *city_object = iter->next(); // GET
		string end_city_name = city_object->getCityName();
		int city_weight = city_object->getWeight();

		// PUT THIS CONNECTION ALL IN ONE OBJECT (CityToCityConnection)
		CityToCityConnection *city_connection = new CityToCityConnection();
		city_connection->addStartCity(start_city_name);
		city_connection->addEndCity(end_city_name);
		city_connection->addWeight(city_weight);

		// ADD CITY CONNECTION TO "PQ" OR SORTED ARRAY. SMALLEST WEIGHT
		// GETS POPPED OUT FIRST
		sorted_array_prims->insert(city_connection);

		// MARK THE FIRST, STARTING NODE AS VISITED
		visited_cities_in_prims[0] = true;
	}

	//--------------------------------------------------------------------
	// IN THIS PART OF THE CODE, I SHOULD HAVE THE SORTED ARRAY POPULATED
	// WITH THE ALL THE CITIES THAT CONNECT TO THE FIRST ITEM CHOSEN.
	// CONNECTIONS ARE SORTED BY WEiGHt
	//--------------------------------------------------------------------

	//-------------------------------------------------------------------
	// WHILE THE SORTED ARRAY IS NOT EMPTY, KEEP TAKING THE SMALLEST AND
	// MAKE MORE CONNECTIONS--ONLY IF IT HAS NOT BEEN VISITED YET
	//-------------------------------------------------------------------
	while (sorted_array_prims->isEmpty() == false)
	{
		//---------------------------------
		// DISPLAY CONTENTS (DELETE LATER)
		//---------------------------------
		/*cout << "\n\nCONTENTS OF PQ\n";
		for (int i = 0; i < sorted_array_prims->size(); i++)
		{
			CityToCityConnection *next_peek = sorted_array_prims->peek(i);
			string start = next_peek->getStartCity();
			string end = next_peek->getEndCity();
			int weight = next_peek->getWeight();
			cout << start << " to " << end << " " << weight << endl;
		}*/

		// WHILE THE SORTED ARRAY IS NOT EMPTY, POP THE SMALLEST
		CityToCityConnection *next_smallest_city_connection = sorted_array_prims->pop();

		// GET THE END CITY OF THAT SMALLEST CITY CONNECTION
		string end_city_name = next_smallest_city_connection->getEndCity();

		//-------------------------------------------------------------
		// CHECK IF THAT END CITY NAME HAS BEEN VISITED. BUT TO DO SO,
		// GET THE INDEX NUMBER CORRELATING WITH THE ARRAY OF VERTICES
		//-------------------------------------------------------------
		int index_of_end_city_correlation_with_boolean_array = 0;
		for (int i = 0; i < current_size_of_array; i++)
		{
			if (end_city_name == array_of_vertex_city_objects[i].getCityName())
			{
				index_of_end_city_correlation_with_boolean_array = i;
			}
		}

		//-------------------------------------------------------------------------
		// IF THIS HAS NOT BEEN VISITED BEFORE, THIS WILL BE YOUR NEXT CONNECTION
		// AND GET EVERYTHING THAT IT CONNECTS TO
		//-------------------------------------------------------------------------
		if (visited_cities_in_prims[index_of_end_city_correlation_with_boolean_array] == false)
		{
			// GO IN AND MARK AS VISITED
			visited_cities_in_prims[index_of_end_city_correlation_with_boolean_array] = true;

			// ADD THE CONNECTION TO THE FINAL RESULT LINKED LIST
			prims_result->add(next_smallest_city_connection);

			// GO TO THE APPROPRIATE ADJACENCY LIST INDEX AND GET ALL THE CONNECIONS OF OF THAT.
			// get(1) GETS THE FIRST CITY OUT OF THAT ADJACENCY LIST'S INDEX
			T *city_object = adjacency_list[index_of_end_city_correlation_with_boolean_array]->get(1);
			string start_city_name = city_object->getCityName();

			//-------------------------------------------------------------------
			// ITERATE THROUGH THE ADJACENCY LISt AND GET ALL THE CONNECTIONS TO
			// PUT IN THE SORTED ARRAY
			//-------------------------------------------------------------------
			ListLinkedIterator<T> *iter = adjacency_list[index_of_end_city_correlation_with_boolean_array]->iterator();

			// PULL OUT THE FIRST ONE SO IT DOES NOT SAY IT CONNECTS TO ITSELF
			iter->next();
			while (iter->hasNext()) // GO THROUGH THE ADJACENCY LIST'S ROW
			{
				T *city_object = iter->next(); // GET
				string end_city_name = city_object->getCityName();
				int city_weight = city_object->getWeight();

				// PUT THIS CONNECTION ALL IN ONE OBJECT (CityToCityConnection)
				CityToCityConnection *city_connection = new CityToCityConnection();
				city_connection->addStartCity(start_city_name);
				city_connection->addEndCity(end_city_name);
				city_connection->addWeight(city_weight);

				// ADD CITY CONNECTION TO "PQ" OR SORTED ARRAY. SMALLEST WEIGHT
				// GETS POPPED OUT FIRST
				sorted_array_prims->insert(city_connection);
			}
		}
	}

	return *prims_result;
}

template <class T>
ListLinked<CityToCityConnection> Graph<T>::kruskal()
{
	//--------------------------------------------------
	// START BY THROWING EVERYTHING IN A QUEUE, SORT IT
	// (I WILL BE STORING IN A SORTED ARRAY)
	//--------------------------------------------------
	for (int i = 0; i < current_size_of_array; i++)
	{
		// GET THE NAME OF THE FIRST NODE OF THE INDEX i
		T *city_object = adjacency_list[i]->get(1);
		string start_city_name = city_object->getCityName();

		ListLinkedIterator<T> *iter = adjacency_list[i]->iterator();
		// PULL OUT THE FIRST ONE SO IT DOES NOT SAY IT CONNECTS TO ITSELF
		iter->next();
		while (iter->hasNext())
		{
			T *city_object = iter->next();
			string end_city_name = city_object->getCityName();
			int city_weight = city_object->getWeight();

			// PUT THIS CONNECTION ALL IN ONE OBJECT (CityToCityConnection)
			CityToCityConnection *city_connection = new CityToCityConnection();
			city_connection->addStartCity(start_city_name);
			city_connection->addEndCity(end_city_name);
			city_connection->addWeight(city_weight);

			// ADD CITY CONNECTION TO "PQ" OR SORTED ARRAY. SMALLEST WEIGHT
			// GETS POPPED OUT FIRST
			sorted_array_kruskals->insert(city_connection);

			// cout << start_city_name << " connects to " << end_node << " and the weight is: " << city_weight << endl;
		}
	}

	//---------------------------------
	// DISPLAY CONTENTS (DELETE LATER)
	//---------------------------------
	/*cout << "CONTENTS OF SORTED ARRAY/PQ\n\n";
	for (int i = 0; i < sorted_array_kruskals->size(); i++)
	{
		CityToCityConnection *next_peek = sorted_array_kruskals->peek(i);
		string start = next_peek->getStartCity();
		string end = next_peek->getEndCity();
		int weight = next_peek->getWeight();
		cout << start << " to " << end << " " << weight << endl;
	}*/

	//-------------------------------------------------------------------
	// INITIALIZE ARRAY OF BOOLEANS. USE TO KEEP TRACK OF VISITED CITIES
	//-------------------------------------------------------------------
	visited_cities_in_kruskals = new bool[current_size_of_array];
	for (int a = 0; a < current_size_of_array; a++)
	{
		visited_cities_in_kruskals[a] = false;
	}

	//------------------------------------
	// INITIALIZE THE QUICK UNION POINTER
	//------------------------------------
	quick_union_pointer = new QuickUnion(current_size_of_array);

	//--------------------------------------------------
	// GET THE SMALLEST ITEM EACH TIME FROM THE LIST
	//--------------------------------------------------
	while (sorted_array_kruskals->isEmpty() == false)
	{
		// POP AN ITEM (WHICH WOULD BE THE NEXT SMALLEST ONE)
		CityToCityConnection *next_smallest_city_connection = sorted_array_kruskals->pop();
		string start_city_name = next_smallest_city_connection->getStartCity();
		string end_city_name = next_smallest_city_connection->getEndCity();
		int weight = next_smallest_city_connection->getWeight();

		//-------------------------------------------------------------
		// GET THE INDEX NUMBER OF THE START CITY
		//-------------------------------------------------------------
		int index_of_start_city_in_array_of_vertices = 0;
		for (int i = 0; i < current_size_of_array; i++)
		{
			if (start_city_name == array_of_vertex_city_objects[i].getCityName())
			{
				index_of_start_city_in_array_of_vertices = i;
			}
		}

		//-------------------------------------------------------------
		// GET THE INDEX NUMBER OF THE END CITY
		//-------------------------------------------------------------
		int index_of_end_city_in_array_of_vertices = 0;
		for (int i = 0; i < current_size_of_array; i++)
		{
			if (end_city_name == array_of_vertex_city_objects[i].getCityName())
			{
				index_of_end_city_in_array_of_vertices = i;
			}
		}

		//--------------------------------------------------
		// CHECK TO SEE IF THE CONNECTION IS ALREADY THERE.
		// IF NOT, MAKE THE CONNECTION
		//--------------------------------------------------
		bool is_connected_already = quick_union_pointer->isConnected(index_of_start_city_in_array_of_vertices, index_of_end_city_in_array_of_vertices);
		if (is_connected_already == false)
		{
			quick_union_pointer->union_(index_of_start_city_in_array_of_vertices, index_of_end_city_in_array_of_vertices);

			// IF start_city_name > end_city_name SWAP THEM SO THAT IT'S
			// IN ALPHABETICAL ORDER WHEN IT OUTPUTS IN THE COMMAND PROMPT
			if (start_city_name > end_city_name) // SWAP NEEDED BEFORE PUTTING IN FINAL FINAL RESULT
			{
				CityToCityConnection *swapped = new CityToCityConnection();
				swapped->addStartCity(end_city_name);
				swapped->addEndCity(start_city_name);
				swapped->addWeight(weight);

				// ADD TO FINAL RESULT (LINKED LIST THAT WILL BE RETURNED)
				kruskals_result->add(swapped);
			}
			else // NO SWAP NEEDED BEFORE PUTTING IN FINAL FINAL RESULT
			{
				// ADD TO FINAL RESULT (LINKED LIST THAT WILL BE RETURNED)
				kruskals_result->add(next_smallest_city_connection);
			}
		}
	}

	return *kruskals_result;
}

template <class T>
ListLinked<CityToCityConnection> Graph<T>::shortestPath(string start_vertex_key, string end_vertex_key)
{
	// THIS IS THE ARRAY THAT GETS UPDATED. THIS IS WHERE THE MAGIC HAPPENS ;)
	CityToCityConnection *smallest_so_far_array = new CityToCityConnection[current_size_of_array];
	for (int k = 0; k < current_size_of_array; k++)
	{
		CityToCityConnection *new_city_connection = new CityToCityConnection();

		// MAKE ALL WEIGHTS MAX INT
		new_city_connection->addWeight(INT_MAX);

		smallest_so_far_array[k] = *new_city_connection;
	}

	//--------------------------------------------------------------
	// CREATE A MATRIX WITH THE SAME SIZE AS THE NUMBER OF VERTICES
	//--------------------------------------------------------------
	int new_matrix[current_size_of_array][current_size_of_array];

	//----------------------------------------------
	// INITIALIZE ALL AREAS OF THE MATRIX WITH MAX
	//----------------------------------------------
	for (int row = 0; row < current_size_of_array; row++)
	{
		for (int column = 0; column < current_size_of_array; column++)
		{
			new_matrix[row][column] = INT_MAX;
		}
	}

	//--------------------------------------------------
	// FIND WHICH ONE start_vertex_key MATCHES WITH IN
	// array_of_vertex_city_objects
	//--------------------------------------------------
	int index_of_start_point;
	for (int i = 0; i < current_size_of_array; i++)
	{
		if (start_vertex_key == array_of_vertex_city_objects[i].getCityName())
		{
			index_of_start_point = i;
		}
	} // HIRSOVA

	//-----------------------------------------------
	// FIND WHICH ONE end_vertex_key MATCHES WITH IN
	// array_of_vertex_city_objects
	//-----------------------------------------------
	int index_of_destination;
	for (int i = 0; i < current_size_of_array; i++)
	{
		if (end_vertex_key == array_of_vertex_city_objects[i].getCityName())
		{
			index_of_destination = i;
		}
	} // PITESTI

	//-----------------------------------
	// GET THE START CITY'S INFORMATION
	// (FIRST ROW OF new_matrix)
	//-----------------------------------
	for (int a = 0; a < current_size_of_array; a++)
	{
		if (adjacency_matrix[index_of_start_point][a] != 0)
		{
			// ADD CONNECTION TO smallest_so_far_array
			CityToCityConnection *new_city_connection = new CityToCityConnection();
			string start_city_name = array_of_vertex_city_objects[index_of_start_point].getCityName();
			string end_city_name = array_of_vertex_city_objects[a].getCityName();
			int city_weight = adjacency_matrix[index_of_start_point][a];

			new_city_connection->addStartCity(start_city_name);
			new_city_connection->addEndCity(end_city_name);
			new_city_connection->addWeight(city_weight);
			smallest_so_far_array[a] = *new_city_connection;

			// ADD TO new_matrix
			new_matrix[0][a] = adjacency_matrix[index_of_start_point][a];

			// ADD CITY CONNECTION TO "PQ" OR SORTED ARRAY. SMALLEST WEIGHT
			// GETS POPPED OUT FIRST
			sorted_array_shortest_path->insert(new_city_connection);
		}
		else
		{
			new_matrix[0][a] = INT_MAX;
		}
	}
	//-----------------------------------
	// SET ITSELF TO 0
	//-----------------------------------
	new_matrix[0][index_of_start_point] = 0;
	CityToCityConnection *new_city_connection = new CityToCityConnection();
	string start_city_name = array_of_vertex_city_objects[index_of_start_point].getCityName();
	string end_city_name = array_of_vertex_city_objects[index_of_start_point].getCityName();
	int city_weight = 0;

	new_city_connection->addStartCity(start_city_name);
	new_city_connection->addEndCity(end_city_name);
	new_city_connection->addWeight(city_weight);
	smallest_so_far_array[index_of_start_point] = *new_city_connection;

	//-------------------------------
	// GO THROUGH THE PRIORITY QUEUE
	//-------------------------------
	int index_counter = 1;
	bool destination_reached = false;
	while (destination_reached == false)
	{
		// PULL OUT THE NEXT CITY CONNECTION WITH THE SMALLEST WEIGHT
		CityToCityConnection *next_smallest_city_connection = sorted_array_shortest_path->pop();
		string start_city_name = next_smallest_city_connection->getStartCity();
		string end_city_name = next_smallest_city_connection->getEndCity();
		int weight = next_smallest_city_connection->getWeight();
		// cout << start_city_name << " connects to " << end_city_name << " and the weight is: " << weight << endl;
		//-----------------------------------------------
		//  GET INDEX OF start_city_name
		//-----------------------------------------------
		int index_of_start_city_name;
		for (int i = 0; i < current_size_of_array; i++)
		{
			if (start_city_name == array_of_vertex_city_objects[i].getCityName())
			{
				index_of_start_city_name = i;
			}
		}
		//-----------------------------------------------
		// GET INDEX OF end_city_name
		//-----------------------------------------------
		int index_of_end_city_name;
		for (int i = 0; i < current_size_of_array; i++)
		{
			if (end_city_name == array_of_vertex_city_objects[i].getCityName())
			{
				index_of_end_city_name = i;
			}
		}

		//----------------------------------------------------------------------------------------------------------------------------

		for (int b = 0; b < current_size_of_array; b++)
		{
			if (adjacency_matrix[index_of_end_city_name][b] != 0)
			{
				// HOW DID I GET TO THIS ROW'S INDEX?
				int weight_to_get_here = smallest_so_far_array[index_of_end_city_name].getWeight();
				int combined_weight = weight_to_get_here + adjacency_matrix[index_of_end_city_name][b];
				if (combined_weight < smallest_so_far_array[b].getWeight()) // UPDATE SMALLEST
				{
					// ADD CONNECTION TO smallest_so_far_array
					CityToCityConnection *new_city_connection = new CityToCityConnection();
					string start_city_name = array_of_vertex_city_objects[index_of_end_city_name].getCityName();
					string end_city_name = array_of_vertex_city_objects[b].getCityName();

					new_city_connection->addStartCity(array_of_vertex_city_objects[index_of_end_city_name].getCityName());
					new_city_connection->addEndCity(end_city_name);
					new_city_connection->addWeight(combined_weight);
					smallest_so_far_array[b] = *new_city_connection;

					// ADD TO new_matrix
					new_matrix[index_counter][b] = combined_weight;

					// ADD CITY CONNECTION TO "PQ" OR SORTED ARRAY. SMALLEST WEIGHT
					// GETS POPPED OUT FIRST
					sorted_array_shortest_path->insert(new_city_connection);
				}
			}
			else
			{
				new_matrix[index_counter][b] = INT_MAX;
			}
		}

		if (index_of_end_city_name == index_of_destination)
		{
			destination_reached = true;
			bool back_to_start = false;
			int index_to_find = index_of_end_city_name;

			// GET THE DESTINATION CONNECTION
			string start_city1 = smallest_so_far_array[index_to_find].getStartCity();
			string end_city1 = smallest_so_far_array[index_to_find].getEndCity();
			shortest_path_result->add(&smallest_so_far_array[index_to_find]);

			for (int r = 0; r < current_size_of_array; r++)
			{
				for (int v = 0; v < current_size_of_array; v++)
				{
					if (smallest_so_far_array[index_to_find].getStartCity() == smallest_so_far_array[v].getEndCity() && smallest_so_far_array[v].getWeight() != 0)
					{
						shortest_path_result->add(&smallest_so_far_array[v]);
						string start = smallest_so_far_array[v].getStartCity();
						string next = smallest_so_far_array[v].getEndCity();
						//-----------------------------------------------
						// GET INDEX OF next
						//-----------------------------------------------
						int index_of_next;
						for (int i = 0; i < current_size_of_array; i++)
						{
							if (next == array_of_vertex_city_objects[i].getCityName())
							{
								index_of_next = i;
							}
						}

						index_to_find = index_of_next;

						if (start == array_of_vertex_city_objects[index_of_destination].getCityName())
						{
							return *shortest_path_result;
						}
						break;
					}
				}
			}
		}

		index_counter++;
	}

	//--------------------------------------------------
	// DISPLAY smallest_so_far_array
	//--------------------------------------------------
	/*cout << right << endl << endl;
	cout << "--------------------------------------------------------------------------------\n";
	cout << "              THIS IS WHAT DIJSKTRA'S smallest_so_far_array LOOKS LIKE: \n";
	cout << "--------------------------------------------------------------------------------\n";
	for (int ii = 0; ii < current_size_of_array; ii++)
	{
		string start_city_name = smallest_so_far_array[ii].getStartCity();
		string end_city_name = smallest_so_far_array[ii].getEndCity();
		int weight_of_this = smallest_so_far_array[ii].getWeight();
		cout << "Index " << setw(2) << ii << ": " << start_city_name << " --> " << end_city_name << " (" << weight_of_this << ")\n";
	}*/

	//--------------------------------------------------
	// DISPLAY MATRIX
	//--------------------------------------------------
	/*cout << left << endl << endl;
	cout << "--------------------------------------------------------------------------------\n";
	cout << "                     THIS IS WHAT DIJKTRA'S MATRIX LOOKS LIKE: \n";
	cout << "--------------------------------------------------------------------------------\n";
	int counter1 = 0;
	for (int row = 0; row < current_size_of_array; row++)
	{
		for (int column = 0; column < current_size_of_array; column++)
		{
			//cout << setprecision(3);
			cout << setw(11) << new_matrix[row][column] << setw(1) << " " ;
			counter1++;

			if (counter1 == current_size_of_array)
			{
				cout << endl << endl << endl << endl;
				counter1 = 0;
			}
		}
	}*/

	return *shortest_path_result;
}

#endif