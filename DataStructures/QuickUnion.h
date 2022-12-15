#if !defined QUICKUNION_H
#define QUICKUNION_H

#include <vector>
#include <iostream>
using namespace std;
class QuickUnion
{
     private:
          vector<int> indices;
  
     public:
          QuickUnion(int num_nodes);
          //~QuickUnion();
          int find(int city);             //returns a subset containing x.
          void union_(int x, int y);  //constructs the union of the disjoint subsets Sx and Sy containing
                                         //x and y, respectively, and adds it to the collection to replace Sx and Sy, which
                                         //are deleted from it.
          bool isConnected(int start_city, int end_city);
          //void printTree();
 };

//-------------
// CONSTRUCTOR
//-------------
QuickUnion::QuickUnion(int num_nodes)
{
     // THIS IS makeSet(int x)
     indices.resize(num_nodes);
     for(int i = 0; i <= indices.size();i++){
     indices[i] = i;
     }
}

//---------------
// FIND THE ROOT
//---------------
int QuickUnion::find(int city)
{
  while(city != indices[city]) {
    city = indices[city];
  }
  return city;
}

//--------------------
// IF THE CONNECTION
// IS ALREADY THERE,
// DISCARD THE ITEM
// RETRIEVED FROM
// THE PRIORITY QUEUE
//--------------------
bool QuickUnion::isConnected(int start_city, int end_city)
{
  return find(start_city) == find(end_city);
}

//--------------------
// THIS CHANGES THE 
// ROOT OF p TO POINT 
// TO THE ROOT OF q
//--------------------
void QuickUnion::union_(int start_city, int end_city)
{
  int i = find(start_city);
  int j = find(end_city);
  indices[i] = j;
}

/*
void QuickUnion::printTree()
{
    for(int i = 0; i <= indices.size(); i++){
        printf("Array[%d] = Parent is %d \n", i ,indices[i]);
    }
}*/
  
  
#endif