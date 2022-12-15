#if !defined SORTEDARRAY_H
#define SORTEDARRAY_H

#include <iostream>
#include <string>
#include "CityToCityConnection.h"
using namespace std;

class SortedArray
{
	private:
		CityToCityConnection **sorted_array;
		int max_size;
		int actual_size_of_array;

	public:
		SortedArray();
		~SortedArray();

		void insert(CityToCityConnection *item);
		CityToCityConnection *pop();
		CityToCityConnection *peek(int index);
		int size();

		void resize(int new_max);
		void displayAll();

		bool isEmpty();
};

SortedArray::SortedArray()
{
	max_size = 2;
	actual_size_of_array = 0;
	sorted_array = new CityToCityConnection*[2];
}

SortedArray::~SortedArray()
{

}

void SortedArray::insert(CityToCityConnection *item)
{
	// IF FULL, RESIZE
	if (actual_size_of_array == max_size)
	{
		resize(max_size * 2);
	}

	// IF IT'S THE FIRST ITEM TO BE ADDED, JUST ADD TO INDEX 0
	if (actual_size_of_array == 0)
	{
		sorted_array[0] = item;
		actual_size_of_array++;
		return;
	}

	// FIRST JUST ADD NEW ITEM TO THE END (RIGHT) OF THE LIST
	sorted_array[actual_size_of_array] = item;

	// SORT THE ARRAY (SHIFTING) WITH FIFO BEHAVIOR
	for (int i = actual_size_of_array; i > 0; i--)
	{
		int new_city_weight = sorted_array[i]->getWeight();
		int old_city_weight = sorted_array[i - 1]->getWeight();

		// IF NEW CITY WEIGHT IS SMALLER, SWAP
		if (new_city_weight < old_city_weight)
		{
			CityToCityConnection *swap_holder = sorted_array[i - 1];
			sorted_array[i - 1] = sorted_array[i];
			sorted_array[i] = swap_holder;
		}
		else
		{
			break;
		}
	}

	// INCREMENT
	actual_size_of_array++;

}


CityToCityConnection* SortedArray::pop()
{
	if (actual_size_of_array == 0)
	{
		return NULL;
	}

	CityToCityConnection *top_item_holder = sorted_array[0];

	// SHIFT EVERYTHING
	for (int i = 0; i < actual_size_of_array; i++)
	{
		sorted_array[i] = sorted_array[i + 1];
	}

	// DECREMENT AND RETURN
	actual_size_of_array--;
	return top_item_holder;
}

CityToCityConnection* SortedArray::peek(int index)
{
	return sorted_array[index];
}

int SortedArray::size()
{
	return actual_size_of_array;
}


void SortedArray::resize(int new_max)
{
	//-----------------------------------
	// CHANGE PRIVATE VARIABLE'S NEW MAX
	//-----------------------------------
	max_size = new_max;

	//-----------------------------------------------------
	// MAKE A TEMPORARY ARRAY THAT WILL HOLD CLASS OBJECTS
	//-----------------------------------------------------
	CityToCityConnection **temp_array = new CityToCityConnection*[max_size];

	//----------------------------------------------------------------------
	// TRANSFER ITEMS FROM FULL ARRAY TO -> THE NEWLY CREATED, BIGGER ARRAY
	//----------------------------------------------------------------------
	for (int i = 0; i < actual_size_of_array; i++)
	{
		temp_array[i] = sorted_array[i];
	}

	delete [] sorted_array;
	sorted_array = temp_array;
}

void SortedArray::displayAll()
{
	for (int i = 0; i < actual_size_of_array; i++)
	{
		CityToCityConnection *city_connecton_popped = sorted_array[i];
		cout << city_connecton_popped->getStartCity() << " to " << city_connecton_popped->getEndCity() << " is " << city_connecton_popped->getWeight() << endl;
	}

}

bool SortedArray::isEmpty()
{
	return (actual_size_of_array == 0);
}





#endif