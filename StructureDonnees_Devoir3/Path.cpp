#include "stdafx.h"
#include "Path.h"
using namespace std;

Path::Path(int size)
{
	this->path = vector<State*>();
	this->lettersCount = vector<int>(size, 0);
	this->cost = 0;
}

Path::Path(const Path& path)
{
	this->path = path.path;
	this->lettersCount = path.lettersCount;
	this->cost = path.cost;
}

Path::~Path()
{
}

int Path::getCost() const
{
	return this->cost;
}

vector<State*>& Path::getPath()
{
	return this->path;
}

vector<State> Path::toVector()
{
	vector<State> v = vector<State>();
	for (size_t i = 0; i < path.size(); i++)
	{
		v.push_back(*path[i]);
	}
	return v;
}

vector<int>& Path::getLettersCount()
{
	return this->lettersCount;
}

void Path::addState(State* state)
{
	path.push_back(state);

	if (path.size() > 1)
	{
		for (Edge transition : state->getTransitions())
		{
			if (transition.getArrivalState()->getId() == path[path.size() - 2]->getId())
			{
				if (transition.getTransition() != "")
				{
					int letterRead = stoi(transition.getTransition()) - 1;
					lettersCount[letterRead]++;
				}
				cost = transition.getWeight();
			}
		}
	}
}

void Path::addFrontState(State* state)
{
	path.insert(path.begin(), state);

	if (path.size() > 1)
	{
		for (Edge transition : path[1]->getTransitions())
		{
			if (transition.getArrivalState()->getId() == state->getId())
			{
				if (transition.getTransition() != "")
				{
					int letterRead = stoi(transition.getTransition()) - 1;
					lettersCount[letterRead]++;
				}
				cost = transition.getWeight();
			}
		}
	}
}