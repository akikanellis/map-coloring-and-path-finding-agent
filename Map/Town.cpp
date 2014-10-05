#include "Map.h"
#include "Town.h"

/*****************************************************************************/
/*                              Constructor                                  */
/*****************************************************************************/
Town::Town()
:
_name(-1),
_state(State::empty),
_color(Color::white)
{}

/*****************************************************************************/
/*                                  Misc                                     */
/*****************************************************************************/
void Town::SetClosestTowns(vector <Town*> towns, vector <vector<double> > mDistance)
{
	// Erase this Town from the vector
	towns.erase(towns.begin() + _name);
	int iterator;

	int elements = towns.size();

	for (int i = 0; i < elements; i++)
	{
		Town* closestTown = this->GetClosestTown(towns, mDistance);
		iterator = closestTown->GetTownIteratorIn(towns);

		//push the closest town that was found to the vector
		_closestTowns.push_back(closestTown);

		towns.erase(towns.begin() + iterator);
	}
}

Town* Town::GetClosestTown(vector <Town*> towns, vector <vector<double> > mDistance)
{
	Town* closestTown;

	closestTown = towns[0];

	for each (Town* town in towns)
	{
		if (mDistance[_name, town->_name] < mDistance[_name, closestTown->_name])
		{
			closestTown = town;
		}
	}
	return closestTown;
}

int Town::GetTownIteratorIn(vector <Town*> towns)
{
	for (int i = 0; i < towns.size(); i++)
	{
		if (towns[i] == this)
		{			
			return i;
		}
	}
}