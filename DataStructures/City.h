/***
 *
 *     CITY CLASS BY: RAYVIN CARL AMADOR
 *     USED FOR CSC2400 PROGRAM 1 BFS/DFS
 *     FALL 2017
 *
 */

#if !defined CITY_H
#define CITY_H

#include <iostream>
#include <string>
using namespace std;

class City
{
private:
	string city_name;
	int weight;
	int visited;

public:
	City();
	~City();

	void addCity(string name);
	string getCityName();
	void changeVisitedStatus(int status);
	int getVisitedStatus();

	void addWeight(int item);
	int getWeight();
};

City::City()
{
	visited = 0;
	weight = 0;
}

void City::changeVisitedStatus(int status)
{
	visited = status;
}

City::~City()
{
}

string City::getCityName()
{
	return city_name;
}

void City::addWeight(int w)
{
	weight = w;
}

int City::getWeight()
{
	return weight;
}

int City::getVisitedStatus()
{
	return visited;
}

void City::addCity(string name)
{
	city_name = name;
}

#endif