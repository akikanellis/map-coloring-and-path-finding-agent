#include "AI.h"

/*****************************************************************************/
/*                              Constructor                                  */
/*****************************************************************************/
AI::AI(Map map, const int speed, const Algorithm algo)
:
_map(map),
_speed(speed),
_algo(algo),
_success(false),
_k(0)
{}

AI::AI(Map map, const int speed, const Algorithm algo, const int k)
:
_map(map),
_speed(speed),
_algo(algo),
_success(false),
_k(k),
_colors(_map._colors)
{
}
/*****************************************************************************/
/*                            Solve Function                                 */
/*****************************************************************************/
void AI::Solve()
{
	auto start = high_resolution_clock::now();

	switch (_algo)
	{
	case AI::BF:
		_success = BestFirstSearch(_map._start);
		break;
	case AI::AS:
		_success = AStar(_map._start);
		break;
	case AI::MC:
		_success = MinConflicts(_map._coloringStart);
		break;
	case AI::BT:
		_success = Backtracking(_map._coloringStart);
		break;
	case AI::BJ:
		_success = CBBackjumping(_map._coloringStart).first;
		break;
	default:
		break;
	}

	auto end = high_resolution_clock::now();

	_performance = end - start;
}
/*****************************************************************************/
/*                   Graph Coloring Algorithm Functions                      */
/*****************************************************************************/
bool AI::MinConflicts(Town* town)
{
	vector<Town*> towns = _map._towns;
	vector<Color> colors = _map._colors;
	random_shuffle(towns.begin(), towns.end());

	for each (Town* town in towns)
	{
		const Color randomColor = colors[rand() % _k];
		town->_color = randomColor;
	}

	const int maxTries = 100;
	const int MaxChanges = 100;

	return MinConflictsHelper(towns, maxTries, MaxChanges);
}

bool AI::Backtracking(Town* town)
{
	for (int i = 0; i < _k; i++)
	{
		const Color color = _colors[i];

		if (IsValid(town, color))
		{
			SetEnvironment(town, color);
			PrintEnvironment();

			if (town == _map._towns[_map._towns.size() - 1])
			{
				return true;
			}

			Town* nextTown = _map._towns[town->_name + 1];

			if (Backtracking(nextTown))
			{
				return true;
			}
			else
			{
				SetEnvironment(town, Color::white);
				PrintEnvironment();
			}
		}
	}
	return false;
}

pair <bool, Town*> AI::CBBackjumping(Town* town)
{
	for (int i = 0; i < _k; i++)
	{
		const Color color = _colors[i];

		if (IsValid(town, color))
		{
			SetEnvironment(town, color);
			PrintEnvironment();

			if (town == _map._towns[_map._towns.size() - 1])
			{
				return pair <bool, Town*>(true, town);
			}

			Town* nextTown = _map._towns[town->_name + 1];

			pair <bool, Town*> result = CBBackjumping(nextTown);

			if (result.first)
			{
				return pair <bool, Town*>(true, town);
			}
			else
			{
				if (result.second != town)
				{
					SetEnvironment(town, Color::white);
					PrintEnvironment();
					town->_conflicts.clear();

					return pair <bool, Town*>(false, result.second);
				}
			}
		}
	}

	SetEnvironment(town, Color::white);
	UpdateAllConflicts(town);

	if (town->_conflicts.empty())
	{
		return pair <bool, Town*>(false, town);
	}

	Town* deepestConflict = GetDeepestConflict(town);
	AddConflicts(deepestConflict, town);

	return pair <bool, Town*>(false, deepestConflict);
}
/*****************************************************************************/
/*                    Graph Coloring Helper Functions                        */
/*****************************************************************************/
bool AI::MinConflictsHelper(vector<Town*> towns, const int maxTries, const int MaxChanges)
{
	for (int i = 0; i < maxTries; i++)
	{
		for (int j = 0; j < MaxChanges; j++)
		{
			vector<Town*> conflicts = GetConflicts(towns);

			if (conflicts.empty())
			{
				return true;
			}
			else
			{
				int randomElement = rand() % conflicts.size();

				do
				{
					randomElement = rand() % conflicts.size();
				} while (randomElement == conflicts.size());

				Town* randomConflict = conflicts[randomElement];
				Town randomConflictCopy = *randomConflict;
				int startingConflicts = GetConflicts(randomConflictCopy);

				Color futureColor = _colors[0];
				randomConflictCopy._color = futureColor;
				int futureConflicts = GetConflicts(randomConflictCopy);

				for (int k = 1; k < _k; k++)
				{
					const Color color = _colors[k];
					randomConflictCopy._color = color;
					const int currentConflicts = GetConflicts(randomConflictCopy);

					if (currentConflicts < futureConflicts)
					{
						futureConflicts = currentConflicts;
						futureColor = color;
					}
				}

				if (futureConflicts < startingConflicts)
				{
					SetEnvironment(randomConflict, futureColor);
					PrintEnvironment();
					if (_speed > 0)
					{
						printf("\nConflicts: %d\nTries #: %d\nChanges #: %d\n", conflicts.size(), i, j);
					}
				}
			}
		}
	}
	return false;
}

int AI::GetConflicts(const Town town)
{
	int conflicts = 0;

	const Color townColor = town._color;
	vector<Town*> neighbors = town._neighbors;

	for each (Town* neighbor in neighbors)
	{
		const Color neighborColor = neighbor->_color;

		if (townColor == neighborColor)
		{
			conflicts++;
		}
	}
	return conflicts;
}

vector<Town*> AI::GetConflicts(const vector<Town*> towns)
{
	vector<Town*> conflicts;

	for each (Town* town in towns)
	{
		const Color townColor = town->_color;
		vector<Town*> neighbors = town->_neighbors;

		for each (Town* neighbor in neighbors)
		{
			const Color neighborColor = neighbor->_color;

			if (townColor == neighborColor && !IsIn(neighbor, conflicts))
			{
				conflicts.push_back(neighbor);
			}
		}
	}
	return conflicts;
}

void AI::UpdateAllConflicts(Town* town)
{
	const vector <Town*> neighbors = town->_neighbors;

	for each (Town* neighbor in neighbors)
	{
		if (neighbor->_color != Color::white && !IsIn(neighbor, town->_conflicts))
		{
			town->_conflicts.push_back(neighbor);
		}
	}
}

void AI::AddConflicts(Town* town, Town* neighbor)
{
	if (neighbor != NULL && town != neighbor)
	{
		town->_conflicts.clear();
		for each (Town* conflict in neighbor->_conflicts)
		{
			if (!IsIn(conflict, town->_conflicts) && conflict != town)
			{
				town->_conflicts.push_back(conflict);
			}
		}
	}
}

Town* AI::GetDeepestConflict(Town* town)
{
	Town* deepestConflict = town->_conflicts[0];

	for (int i = 1; i < town->_conflicts.size(); i++)
	{
		Town* conflict = town->_conflicts[i];

		if (deepestConflict->_name < conflict->_name)
		{
			deepestConflict = conflict;
		}
	}
	return deepestConflict;
}

bool AI::IsValid(Town* town, const Color color)
{
	const vector <Town*> neighbors = town->_neighbors;

	for each (Town* t in neighbors)
	{
		const Color neighborColor = t->_color;

		if (neighborColor == color)
		{
			return false;
		}
	}
	return true;
}

bool AI::IsIn(const Town* town, const vector<Town*> towns)
{
	for each (const Town* t in towns)
	{
		if (town == t)
		{
			return true;
		}
	}
	return false;
}
/*****************************************************************************/
/*                      Search Algorithm Functions                           */
/*****************************************************************************/
bool AI::BestFirstSearch(Town* town)
{
	AI::OrderByHeuristic comparator(*this);
	priority_queue<Town*, vector<Town*>, AI::OrderByHeuristic> priorityQueue(comparator);

	priorityQueue.push(town);

	SetEnvironment(town, State::visited);

	while (!priorityQueue.empty())
	{
		town = priorityQueue.top();
		priorityQueue.pop();

		SetEnvironment(town, State::visited);
		PrintEnvironment();

		if (town == _map._exit)
		{
			return true;
		}

		vector <Town*> neighbors = town->_neighbors;

		for each (Town* neighbor in neighbors)
		{
			if (neighbor->_state == State::town)
			{
				neighbor->_parent = town;

				priorityQueue.push(neighbor);
			}
		}
	}
	return false;
}


bool AI::AStar(Town* town)
{
	AI::OrderByTotalCost comparator;
	vector<TownWithCost> priorityQueue;

	TownWithCost currentTown = { town, 0 };

	priorityQueue.push_back(currentTown);

	SetEnvironment(currentTown.town, State::visited);

	while (!priorityQueue.empty())
	{
		currentTown = priorityQueue.front();
		town = currentTown.town;
		priorityQueue.erase(priorityQueue.begin());

		SetEnvironment(town, State::visited);
		PrintEnvironment();

		if (town == _map._exit)
		{
			return true;
		}

		vector <Town*> neighbors = town->_neighbors;

		for each (Town* neighbor in neighbors)
		{
			if (neighbor->_state == State::town)
			{
				neighbor->_parent = town;
				TownWithCost neighborWithCost = { neighbor, GetTotalCost(neighbor) };

				priorityQueue.push_back(neighborWithCost);
			}
		}
		make_heap(priorityQueue.begin(), priorityQueue.end(), comparator);
	}
	return false;
}

//bool AI::AStar(Town* town)
//{
//	AI::OrderByTotalCost comparator;
//	vector<TownWithCost> priorityQueue;
//
//	vector<int> indexes;
//	int currentIndex = town->_name;
//
//	indexes.push_back(currentIndex);
//
//	TownWithCost currentPath = { indexes, 0 };
//	priorityQueue.push_back(currentPath);
//
//	SetEnvironment(town, State::visited);
//
//	while (!priorityQueue.empty())
//	{
//		currentPath = priorityQueue.front();
//		currentIndex = currentPath.indexes.back();
//		Town temp = *_map._towns[currentIndex];
//		priorityQueue.erase(priorityQueue.begin());
//
//		SetEnvironment(temp, State::visited);
//		PrintEnvironment();
//
//		if (temp._name == _map._exit->_name)
//		{
//			return true;
//		}
//
//		vector <Town*> neighbors = town->_neighbors;
//
//		for each (Town* neighbor in neighbors)
//		{
//			Town tempNeighbor = *neighbor;
//			int neighborIndex = tempNeighbor._name;
//
//			if (tempNeighbor._state == State::town)
//			{
//
//				//TownWithCost neighborWithCost = { tempNeighbor, GetTotalCost(tempNeighbor) };
//
//				//priorityQueue.push_back(neighborWithCost);
//			}
//		}
//		make_heap(priorityQueue.begin(), priorityQueue.end(), comparator);
//	}
//	return false;
//}
/*****************************************************************************/
/*                  Search Algorithm Helper Functions                        */
/*****************************************************************************/
double AI::GetHeuristic(const Town* town)
{
	return _map._mDistance[town->_name][_map._exit->_name];
}

double AI::GetHeuristic(const Town town)
{
	return _map._mDistance[town._name][_map._exit->_name];
}

double AI::GetTotalCost(Town* town)
{
	double cost = 0;
	const double heuristicCost = GetHeuristic(town);

	// Compute real cost
	for (Town* t = town; t != _map._start; t = t->_parent)
	{		
		cost += _map._adjacency[t->_name][t->_parent->_name];
	}

	// Add heuristic cost
	cost += heuristicCost;

	return cost;
}

/*****************************************************************************/
/*                                 Misc                                      */
/*****************************************************************************/
void AI::SetEnvironment(Town* town, State state)
{
	town->_state = state;
	_map._map[town->_row][town->_column]._state = state;
}

void AI::SetEnvironment(Town town, State state)
{
	_map._map[town._row][town._column]._state = state;
}

void AI::SetEnvironment(Town* town, Color color)
{
	town->_color = color;
	_map._map[town->_row][town->_column]._color = color;
}

void AI::PrintEnvironment()
{
	if (_speed > 0)
	{
		if (_k == 0)
		{
			_map.PathPrintAll();
		}
		else
		{
			_map.ColorPrintAll();
		}
		Sleep(_speed);
	}
}
