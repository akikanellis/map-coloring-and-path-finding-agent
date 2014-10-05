#include "Line.h"

#include <vector>

#pragma once
using namespace std;

/**
 *enum char class for keeping the state of each town as both a enum word and a char
 *
 *@wall		- town is a wall and can't be traversed
 *@empty	- town is empty and can be traversed
 *@visited	- town has already been visited
 *@deadEnd	- town has already been visited and leads to a dead end
 *@solution	- town leads to the solution that was found found
 */
enum class State : char
{
	empty		= '.',
	town		= 'o',
	visited		= '*',
	solution	= '.'
};

enum class Color : char
{
	white	= 'w',
	red		= 'r',
	green	= 'g',
	blue	= 'b',
	yellow	= 'y'
};

class Map;

/**
 *Town is the data structure used to represent each room in the map
 *
 *@_parent		- stores the town visited prior to visiting the current town
 *@_row			- row of the map vector that the town corresponds to
 *@_column		- column of the map vector that the town corresponds to
 *@_neighbors	- a vector of cells that this town is connected with
 *@_state		- the state that the town is at
 */
class Town
{
/*****************************************************************************/
/*                        User Defined Variables                             */
/*****************************************************************************/
public:
	bool operator==(const Town* rhs)
	{
		return (_name == rhs->_name);
	}

	bool operator!=(const Town* rhs)
	{
		return (_name != rhs->_name);
	}

	bool operator==(const Town &rhs)
	{
		return (_name == rhs._name);
	}

	bool operator!=(const Town &rhs)
	{
		return (_name != rhs._name);
	}
/*****************************************************************************/
/*                               Functions                                   */
/*****************************************************************************/
public:
	Town();

	void SetClosestTowns(vector <Town*> towns, vector <vector<double> > mDistance);
	Town* GetClosestTown(vector <Town*> towns, vector <vector<double> > mDistance);

	int GetTownIteratorIn(vector <Town*> towns);
/*****************************************************************************/
/*                            Class Variables                                */
/*****************************************************************************/
public:
	int _name;

	Town* _parent;

	int _row;
	int	_column;

	vector<Town*> _closestTowns;
	vector<Town*> _neighbors;
	vector<Town*> _conflicts;
	State _state;
	Color _color;
};
