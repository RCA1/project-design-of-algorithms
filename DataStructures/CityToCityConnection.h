#if !defined CITYTOCITYCONNECTION_H
#define CITYTOCITYCONNECTION_H

#include <iostream>
#include <string>
using namespace std;

class CityToCityConnection
{
	private:
		string start_city;
		string end_city;
		int weight;

	public:
		CityToCityConnection();
		~CityToCityConnection();

		void addStartCity(string start);
		string getStartCity();

		void addEndCity(string end);
		string getEndCity();

		void addWeight(int city_weight);
		int getWeight();
};

CityToCityConnection::CityToCityConnection()
{
	start_city = "NULL";
	end_city = "NULL";
	weight = 0;
}

CityToCityConnection::~CityToCityConnection()
{

}

void CityToCityConnection::addStartCity(string start)
{
	start_city = start;
}

string CityToCityConnection::getStartCity()
{
	return start_city;
}

void CityToCityConnection::addEndCity(string end)
{
	end_city = end;
}

string CityToCityConnection::getEndCity()
{
	return end_city;
}

void CityToCityConnection::addWeight(int city_weight)
{
	weight = city_weight;
}

int CityToCityConnection::getWeight()
{
	return weight;
}

#endif