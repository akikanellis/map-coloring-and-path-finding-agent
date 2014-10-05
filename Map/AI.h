#include "Map.h"

#include <stdio.h>

#include <chrono>
#include <iostream>
#include <deque>
#include <queue>
#include <sstream>


#pragma once

using namespace std::chrono;


/**
 *AI is the class that will solver the map vector through the search algorithm
 *that was given
 *
 *@mazeCopy		- a copy of the Map object that was given to the constructor
 *@helperPath	- helper path for DFS and IDS
 *@finalPath	- final path that will be printed on the map
 *@speed		- speed of the agent (in milliseconds, higher is slower)
 */
class AI
{
/*****************************************************************************/
/*                        User Defined Variables                             */
/*****************************************************************************/
public:
	/**
	 *enum for storing the choise given in the main function. It's also used for
	 *the Solve function to choose which algorithm to implement
	 */
	enum Algorithm
	{
		MC = 1,
		BT = 2,
		BJ = 3,
		BF = 4,
		AS = 5
	};
private:
	struct TownWithCost
	{
		Town* town;
		double cost;
	};
	
	/**
	*Comparator is used to overload the "()" operator. That is done with the
	*goal of having our priority queue in UCS sorting the items by checking
	*the cost of each CellWithCost element
	*/
	struct OrderByHeuristic
	{
		AI &outer;

		OrderByHeuristic(AI &o) :outer(o){}

		bool operator()(const Town* lfs, const Town* rhs)const
		{
			return outer.GetHeuristic(lfs) > outer.GetHeuristic(rhs);
		}
	};

	/*struct OrderByTotalCost
	{
		AI &outer;

		OrderByTotalCost(AI &o) :outer(o){}

		bool operator()(Town* lfs, Town* rhs)
		{
			return outer.GetTotalCost(lfs) > outer.GetTotalCost(rhs);
		}
	};*/

	struct OrderByTotalCost
	{
		bool operator()(TownWithCost lfs, TownWithCost rhs)
		{
			return lfs.cost > rhs.cost;
		}
	};
/*****************************************************************************/
/*                               Functions                                   */
/*****************************************************************************/
public:
	/**
	 *The AI constructor copies the Map object to a new Map object and also
	 *defines the speed of the agent
	 *
	 *@mazeObj		- the Map object given
	 *@givenSpeed	- the agent's given speed (in milliseconds, higher is slower)
	 */
	AI(Map map, const int speed, const  Algorithm algo);
	AI(Map map, const int speed, const  Algorithm algo, const int k);

	/**
	 *Solve runs the search algorithm chosen and then calls the CopyPath function
	 *in order to copy the path found to the finalPath deque
	 *
	 *@algo - the algorithm that was chosen
	 */
	void Solve();
private:
	bool MinConflicts(Town* town);
	bool Backtracking(Town* town);
	pair <bool, Town*> CBBackjumping(Town* town);
	bool BestFirstSearch(Town* town);
	bool AStar(Town* town);

	bool MinConflictsHelper(vector<Town*> towns, const int maxTries, const int MaxChanges);

	int GetConflicts(const Town town);
	vector<Town*> GetConflicts(const vector<Town*> towns);
	void UpdateAllConflicts(Town* town);
	void AddConflicts(Town* town, Town* neighbor);
	Town* GetDeepestConflict(Town* town);

	bool IsValid(Town* town, const Color color);
	bool IsIn(const Town* town, const vector<Town*> towns);



	double GetHeuristic(const Town* town);
	double GetHeuristic(const Town town);
	/**
	 *Returns the total cost from start to town
	 *
	 *@town		- the input town
	 *@return	- total path cost
	 */
	double GetTotalCost(Town* town);
	double GetTotalCost(Town town);

	/**
	 *Sets the environment to the desired state
	 *
	 *@town		- the input town
	 *@state	- the desired state
	 */
	void SetEnvironment(Town* town, State state);
	void SetEnvironment(Town  town, State state);
	void SetEnvironment(Town* town, Color color);
	/**
	 *Prints the environment in a speed based on the speed given
	 */
	void PrintEnvironment();

	/**
	 *Prints the solution path on the map by setting the path found to solution
	 *and then printing that envirnment based on the speed given
	 */
	void ColorPrintEnvironment();
	void PathPrintEnvironment();
/*****************************************************************************/
/*                            Class Variables                                */
/*****************************************************************************/
public:
	bool _success;
	high_resolution_clock::duration _performance;
private:
	Map _map;
	const int _k;
	const Algorithm _algo;
	
	vector <Color> _colors;

	const int _speed;
};

