/***
 *
 *     PROGRAM #1 BY: RAYVIN CARL AMADOR
 *
 *     TO COMPILE THIS PROGRAM:
 *        1.) g++ GraphDriver.cpp -o MinSpan
 *        2.) MinSpan vertex_file.txt edge_file.txt (FOR PRIM'S AND KRUSKAL'S)
 *        3.) MinSpan romanian_cities.txt romanian_mileages.txt Hirsova Pitesti (FOR DIJKSTRA'S)
 *
 */

#include "Graph.h"

int main(int argc, char *argv[])
{
	/*********************************************************************************
	 *          CREATE AN INSTANCE OF THE GRAPH CLASS AND IT MUST ACCEPT THE MAXIMUM
	 *          NUMBER OF VERTICES ALLOWED IN THE GRAPH. START WITH SIZE 2 AND
	 *          RESIZE (DOUBLE IT) WHENEVER IT IS FULL
	 ********************************************************************************/

	// RETURNS A POINTER TO THE NEWLY CREATED CLASS
	// PASS 2 TO GRAPH'S CONSTRUCTOR SO THAT THE ARRAY
	// SIZE STARTS WITH A SIZE OF 2 AND IT SHOULD
	// DYNAMICALLY RESIZE, DOUBLING EVERY TIME IT
	// GETS FULL
	Graph<City> *graph_pointer = new Graph<City>(2);

	/****************************************************************************************************************************
	 *          FROM THE COMMAND PROMPT, READ 2 TEXT FILES
	 *
	 *          THE FIRST FILE CONTAINS THE VERTICES/NODES OF THE GRAPH. ADD EACH ONE OF THOSE TO A PRIVATE ARRAY IN GRAPH
	 *
	 *          THE SECOND FILE CONTAINS THE EDGES. PASS EACH LINE WHICH CONTAINS 3 THINGS AND ADD THE EDGES TO THE ADJACENCY
	 *          MATRIX AND ADJACENCY LIST. COMPARE AND FIND WHICH INDEX OF THE array_of_vertices THE STRING KEYS MATCH WITH.
	 *
	 *          FOR EXAMPLE IF COOKEVILLE WAS THE START CITY, NASHVILLE WAS THE END CITY, AND THE WEIGHT WAS 60--
	 *          AND IN THE ARRAY OF VERTICES COOKEVILLE WAS IN INDEX 4 AND NASHVILLE WAS IN INDEX 10, YOU WOULD GO TO THE
	 *          MATRIX, GO TO ROW 4 COLUMN 10 AND PUT A 60 THERE
	 ****************************************************************************************************************************/

	//---------------------------------------------------
	// READS FIRST FILE
	//
	// THAT WAS READ FROM COMMAND LINE
	//---------------------------------------------------
	cout << endl;
	cout << "--------------------------------------------------------------------------------\n";
	cout << "                     ADDING ITEMS TO THE ARRAY OF VERTICES\n";
	cout << "--------------------------------------------------------------------------------\n";

	cout << left;
	ifstream input_file1(argv[1]);
	if (!input_file1.is_open())
	{
		cout << "ERROR OPENING THE FIRST FILE\n";
		return 0;
	}

	// HOLDS WHAT THE FILE READS PER LINE
	string city_name;

	// GO THROUGH THE FIRST FILE
	while (input_file1 >> city_name)
	{
		// CREATE NEW VARIABLE THAT HOLDS A CITY OBJECT
		City *city_object = new City();

		// GET THE NAME OF THE CITY FROM THAT CITY OBJECT
		city_object->addCity(city_name);

		// DEREFERENCED WITH * SO THAT I DON'T HAVE TO MAKE
		// addVertex(T item) to addVertex(T* item)
		graph_pointer->addVertex(*city_object);
	}

	// CLOSE THE FILE
	input_file1.close();

	//---------------------------------------------------
	// READS SECOND FILE
	//
	// THAT WAS READ FROM COMMAND LINE
	//---------------------------------------------------
	ifstream input_file2(argv[2]);
	if (!input_file2.is_open())
	{
		cout << "ERROR OPENING THE SECOND FILE\n";
		return 0;
	}

	// VARIABLE DEFINITIONS
	string start_city;
	string end_city;
	double edge_weight;

	// READ FIRST ITEM IN THE LIST WHICH CONTAINS 3 THINGS AND CALL ADD EDGE.
	// DO THIS FIRST TO MAKE SURE IT'S NOT EMPTY
	input_file2 >> start_city;
	input_file2 >> end_city;
	input_file2 >> edge_weight;
	graph_pointer->addEdge(start_city, end_city, edge_weight);

	// GO THROUGH THE SECOND FILE
	while (!input_file2.eof())
	{
		input_file2 >> start_city;
		input_file2 >> end_city;
		input_file2 >> edge_weight;
		graph_pointer->addEdge(start_city, end_city, edge_weight);
	}

	// CLOSE THE FILE
	input_file2.close();

	/**************************************************
	 * COUT CHECK. COMMENT/DELETE THIS LATER
	 *************************************************/
	graph_pointer->displayMatrix();
	graph_pointer->displayAdjacencyList();
	graph_pointer->displayAdjacencyListWeights();

	/**********************************
	 * GET PRIM'S
	 *********************************/
	ListLinked<CityToCityConnection> prims_result = graph_pointer->prim();
	ListLinkedIterator<CityToCityConnection> *iter_prims = prims_result.iterator();
	cout << endl;
	cout << "--------------------------------------------------------------------------------\n";
	cout << "                                PRIM'S ALGORITHM:\n";
	cout << "--------------------------------------------------------------------------------\n";
	while (iter_prims->hasNext())
	{
		CityToCityConnection *city_connection = iter_prims->next();
		string start = city_connection->getStartCity();
		string end = city_connection->getEndCity();
		int weight = city_connection->getWeight();

		cout << setw(22) << " " << setw(15) << start << " -->   " << setw(15) << end << " " << weight << endl;
	}

	/**********************************
	 * GET KRUSKAL'S
	 *********************************/
	ListLinked<CityToCityConnection> kruskals_result = graph_pointer->kruskal();
	ListLinkedIterator<CityToCityConnection> *iter_kruskals = kruskals_result.iterator();
	cout << endl;
	cout << "--------------------------------------------------------------------------------\n";
	cout << "                               KRUSKAL'S ALGORITHM:\n";
	cout << "--------------------------------------------------------------------------------\n";
	while (iter_kruskals->hasNext())
	{
		CityToCityConnection *city_connection = iter_kruskals->next();
		string start = city_connection->getStartCity();
		string end = city_connection->getEndCity();
		int weight = city_connection->getWeight();

		cout << setw(22) << " " << setw(15) << start << " -->   " << setw(15) << end << " " << weight << endl;
	}

	// ListLinked<CityToCityConnection> kruskals_result = graph_pointer->kruskal();
	// ListLinkedIterator<CityToCityConnection>* iter_final = kruskals_result.iterator();

	/**********************************
	 * GET SHORTEST PATH ONLY IF THEY
	 * TYPE A START AND END CITY IN
	 * THE COMMAND PROMPT
	 *********************************/
	if (argv[3] != NULL && argv[4] != NULL)
	{
		//---------------------------------------------------
		// READS 3rd ARGUMENT FORM COMMAND LINE
		//
		//
		//---------------------------------------------------
		string start_vertex_key = argv[3];

		//---------------------------------------------------
		// READS 4th ARGUMENT FORM COMMAND LINE
		//
		//
		//---------------------------------------------------
		string end_vertex_key = argv[4];

		ListLinked<CityToCityConnection> shortest_path_result = graph_pointer->shortestPath(start_vertex_key, end_vertex_key);
		int size_of_shortest_path = shortest_path_result.size();
		CityToCityConnection *temp_array[size_of_shortest_path];
		ListLinkedIterator<CityToCityConnection> *iter_shortest_path = shortest_path_result.iterator();
		int temp_counter = 0;

		// PUT THE STUFF THAT'S IN THE LINKED LIST INTO AN ARRAY
		// SO THAT I CAN RETRIEVE THE ITEMS IN THE ORDER THAT I WANT
		while (iter_shortest_path->hasNext())
		{
			temp_array[temp_counter] = iter_shortest_path->next();
			temp_counter++;
		}

		// GET TOTAL VALUE
		int total_cost_from_start_to_end = temp_array[0]->getWeight();

		cout << endl;
		cout << "--------------------------------------------------------------------------------\n";
		cout << "                              DIJKSTRA'S ALGORITHM:\n";
		cout << "--------------------------------------------------------------------------------\n";
		cout << setw(22) << " "
			 << "Traveling from " << start_vertex_key << " to " << end_vertex_key << endl;
		cout << setw(22) << " "
			 << "TOTAL COST: " << total_cost_from_start_to_end << endl;

		for (int i = size_of_shortest_path - 1; i >= 0; i--)
		{
			CityToCityConnection *city_connection = temp_array[i];
			string start = city_connection->getStartCity();
			string end = city_connection->getEndCity();
			int weight = city_connection->getWeight();

			cout << setw(22) << " " << setw(15) << start << " -->   " << setw(15) << end << " " << weight << endl;
		}
	}

	/**********************************
	 * CALL DESTRUCTOR
	 *********************************/
	graph_pointer->~Graph();

	cout << endl;
	cout << setw(22) << " "
		 << "DONE!\n\n";

	return 0;
}