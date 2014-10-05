#include "Town.h"

#include <time.h>
#include <windows.h>

#include <iostream>
#include <algorithm>


#pragma once
using namespace std;

/**
 *Map is the class that will construct the map vector, define each town's
 *neighbors, define the costs of the links and the _start and end of the map
 *
 *@_map		- a 2D Town vector which holds the cells
 *@height	- final height of the vector
 *@width	- final width of the vector
 *@_start	- starting point of the map (top left corner)
 *@_exit		- ending point of the map (bottom right corner)
 *@order	- used by DefineNeighbors() to know in which order to push() the
 *            cells in the neighbors vector, it will push them in reverse order
 *			  example: If you want a town to check its children through the
 *					   order: right, down, left, up then you have to initialize
 *					   the vector in reverse like this
 *					   order({ { up, left, down, right } })
 */
class Map
{
/*****************************************************************************/
/*                        User Defined Variables                             */
/*****************************************************************************/
public:
/*****************************************************************************/
/*                               Functions                                   */
/*****************************************************************************/
public:
	/**
	 *The Map constructor initializes the width and height to the given ones,
	 *initializes the order of town checking, feeds the srand with the clock
	 *of the computer, resizes the vector to the dimensions given,
	 *initializes the properties row, column and state of each town and then
	 *generates the map and defines its neighbors
	 *
	 *@givenWidth	-	the width given to the constructor
	 *@givenHeight	-	the height given to the constructor
	 */
	Map(const int elements, const int size, const int seed);


	/**
	 *Used to print the map array
	 */
	void ColorPrintAll();
	void ColorPrintMap();
	void ColorPrintTowns();
	void ColorPrintConnections();

	void PathPrintAll();
	void PathPrintMap(); 
	void PathPrintTowns();
	void PathPrintConnections();
private:
	void InitialiseVectors();
	/**
	 *Generates the map by producing a random odd row, a random odd column,
	 *changing the state of the random town chosen to empty and then feeding
	 *that town to the recursive function
	 */
	void GenerateMap();
	void SetDistance();

	void InitialiseClosestTowns();
	void GenerateLinksAndWeights();

	bool MakeConnection(Town* randomTown);
	void ErasePossibleConnection(Town* first, Town* second);
	bool Intersects(Line line);
/*****************************************************************************/
/*                            Class Variables                                */
/*****************************************************************************/
public:	
	vector <vector<Town> > _map;
	vector <Town*> _towns;
	vector <vector<int> > _adjacency;
	vector <vector<double> > _mDistance;
	vector <Line> _edges;

	vector<Color> _colors;

	const int _elements;
	const int _size;

	Town* _start;
	Town* _exit;

	Town* _coloringStart;
};