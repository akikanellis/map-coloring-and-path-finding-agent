#include "Map.h"

/*****************************************************************************/
/*                              Constructor                                  */
/*****************************************************************************/
Map::Map(const int elements, const int size, const int seed)
:
_elements(elements),
_size(size)
{
	srand(seed);

	InitialiseVectors();
	GenerateMap();
	SetDistance();

	InitialiseClosestTowns();

	GenerateLinksAndWeights();

	const int randomStart = rand() % _elements;

	int randomExit;

	do
	{
		randomExit = rand() % _elements;
	} while (randomExit == randomStart);

	_start = _towns[randomStart];
	_exit = _towns[randomExit];

	_coloringStart = _towns[0];
}

/*****************************************************************************/
/*                          Generator Functions                              */
/*****************************************************************************/
void Map::InitialiseVectors()
{
	_map.resize(_size);
	for (int i = 0; i < _size; i++)
	{
		_map[i].resize(_size);
	}

	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; j++)
		{
			_map[i][j]._row = i;
			_map[i][j]._column = j;
		}
	}

	_adjacency.resize(_elements);
	for (int i = 0; i < _elements; i++)
	{
		_adjacency[i].resize(_elements);
		for (int j = 0; j < _elements; j++)
		{
			_adjacency[i][j] = -1;
		}
	}

	for (int i = 0; i < _elements; i++)
	{
		_adjacency[i][i] = 0;
	}

	_colors.push_back(Color::red);
	_colors.push_back(Color::green);
	_colors.push_back(Color::blue);
	_colors.push_back(Color::yellow);
}

void Map::GenerateMap()
{
	Town* town;
	int name = 0;

	int row;
	int column;

	for (int i = 0; i < _elements; i++)
	{
		do
		{
			row = rand() % _size;
			column = rand() % _size;
			town = &_map[row][column];
		} while (town->_state != State::empty);

		town->_name = name++;
		town->_state = State::town;
		_towns.push_back(town);
	}
}

void Map::SetDistance()
{
	for (int i = 0; i < _elements; i++)
	{
		_mDistance.resize(_elements);
		for (int j = 0; j < _elements; j++)
		{
			_mDistance[i].resize(_elements);

			const double distance = Line::GetDistance(_towns[i], _towns[j]);
			_mDistance[i][j] = distance;
		}
	}
}

void Map::GenerateLinksAndWeights()
{
	vector <Town*> remainingTowns = _towns;
	int randomName;

	while (!remainingTowns.empty())
	{
		bool connectionMade;
		random_shuffle(remainingTowns.begin(), remainingTowns.end());
		Town* randomTown = remainingTowns.front();

		connectionMade = MakeConnection(randomTown);

		if (!connectionMade)
		{
			const int iterator = randomTown->GetTownIteratorIn(remainingTowns);
			remainingTowns.erase(remainingTowns.begin() + iterator);
		}
	}
}

bool Map::MakeConnection(Town* randomTown)
{
	Town* closestTown;

	while (!randomTown->_closestTowns.empty())
	{		
		closestTown = randomTown->_closestTowns.front();

		Line line(randomTown, closestTown);

		if (Intersects(line))
		{
			ErasePossibleConnection(randomTown, closestTown);
			continue;
		}
		
		const int randomCost = (rand() % 99) + 1;

		_adjacency[randomTown->_name][closestTown->_name] = randomCost;
		_adjacency[closestTown->_name][randomTown->_name] = randomCost;

		_edges.push_back(line);
		randomTown->_neighbors.push_back(closestTown);
		closestTown->_neighbors.push_back(randomTown);

		ErasePossibleConnection(randomTown, closestTown);

		return true;
	}
	return false;
}

bool Map::Intersects(Line line)
{
	for each (Line l in _edges)
	{
		if (Line::AreIntersected(line, l))
		{
			return true;
		}
	}
	return false;
}

/*****************************************************************************/
/*                        Town Defining Functions                            */
/*****************************************************************************/
void Map::ErasePossibleConnection(Town* first, Town* second)
{
	first->_closestTowns.erase(first->_closestTowns.begin());

	int iterator = first->GetTownIteratorIn(second->_closestTowns);

	second->_closestTowns.erase(second->_closestTowns.begin() + iterator);
}

void Map::InitialiseClosestTowns()
{
	// Initialising the vector which contains the closest towns for each town
	for each (Town* town in _towns)
	{
		town->SetClosestTowns(_towns, _mDistance);
	}
}
/*****************************************************************************/
/*                                 Misc                                      */
/*****************************************************************************/
void Map::ColorPrintAll()
{
	system("CLS");
	ColorPrintMap();
	ColorPrintTowns();
	ColorPrintConnections();
}

void Map::ColorPrintMap()
{
	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; ++j)
		{
			if (_map[i][j]._state == State::empty)
			{
				cout << (char)_map[i][j]._state;
			}
			else
			{
				cout << (char)_map[i][j]._color;
			}
		}
		cout << endl;
	}
	cout << endl;
}

void Map::ColorPrintTowns()
{
	const int numOfStars = 21;

	cout << endl;
	printf("Town \t(x,y) \tColor");
	cout << endl;
	for (int i = 0; i < numOfStars; i++)
	{
		printf("*");
	}
	cout << endl;

	for each (Town* town in _towns)
	{
		const int name = town->_name;
		const Color color = town->_color;

		const int x = town->_row;
		const int y = town->_column;

		printf("  %d \t(%d,%d) \t  %c", name, x, y, color);
		cout << endl;
	}
	for (int i = 0; i < numOfStars; i++)
	{
		printf("*");
	}
	cout << endl;
}

void Map::ColorPrintConnections()
{
	const int numOfStars = 22;

	cout << endl;
	printf("Start \tEnd \tColors");
	cout << endl;
	for (int i = 0; i < numOfStars; i++)
	{
		printf("*");
	}
	cout << endl;

	for each (Line line in _edges)
	{
		const int firstName = line._A->_name;
		const int secondName = line._B->_name;

		const Color firstColor = line._A->_color;
		const Color secondColor = line._B->_color;

		printf("  %d \t %d \t(%c,%c)", firstName, secondName, firstColor, secondColor);
		cout << endl;
	}
	for (int i = 0; i < numOfStars; i++)
	{
		printf("*");
	}
	cout << endl;
}

void Map::PathPrintAll()
{
	system("CLS");
	PathPrintMap();
	PathPrintTowns();
	PathPrintConnections();
}

void Map::PathPrintMap()
{
	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; ++j)
		{
			cout << (char)_map[i][j]._state;
		}
		cout << endl;
	}
	cout << endl;
}

void Map::PathPrintTowns()
{
	const int numOfStars = 20;

	cout << endl;
	printf("Town \t(x,y) \tState \t heurCost");
	cout << endl;
	for (int i = 0; i < numOfStars; i++)
	{
		printf("*");
	}
	cout << endl;

	for each (Town* town in _towns)
	{
		const int name = town->_name;

		const int x = town->_row;
		const int y = town->_column;

		const State state = town->_state;

		const double heurCost = _mDistance[name][_exit->_name];

		printf("%d \t(%d,%d) \t%c \t%.2f", name, x, y, state, heurCost);
		cout << endl;
	}
	for (int i = 0; i < numOfStars; i++)
	{
		printf("*");
	}
	cout << endl;
}

void Map::PathPrintConnections()
{
	const int numOfStars = 20;

	cout << endl;
	printf("Start \tEnd \tReal Cost \tHeuristic Cost");
	cout << endl;
	for (int i = 0; i < numOfStars; i++)
	{
		printf("*");
	}
	cout << endl;

	for each (Line line in _edges)
	{
		const int firstName	= line._A->_name;
		const int secondName = line._B->_name;

		const int cost = _adjacency[firstName][secondName];

		printf("%d \t%d \t%d", firstName,secondName, cost);
		cout << endl;
	}
	for (int i = 0; i < numOfStars; i++)
	{
		printf("*");
	}
	cout << endl;
}