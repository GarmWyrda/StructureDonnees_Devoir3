#include "stdafx.h"
#include "AFDGraph.h"


AFDGraph::AFDGraph()
{
	this->idStartState = -1;
	this->states = vector<State>();
}


AFDGraph::~AFDGraph()
{
}

void AFDGraph::addNewState(int id, bool final, bool start)
{
	State newState = State(id, final);
	this->states.push_back(newState);
	if (start) {
		this->idStartState = this->states.size() - 1;
	}
}

const State & AFDGraph::getState(int i) const
{
	return this->states[i];
}

State & AFDGraph::getState(int id)
{
	vector<State>::iterator it;
	for (it = this->states.begin(); it != this->states.end(); ++it) {
		if (it->getId() == id) {
			break;
		}
	}
	return *it;
}

const State & AFDGraph::getState(const State & state) const
{
	vector<State>::const_iterator it;
	for (it = this->states.cbegin(); it != this->states.cend(); ++it) {
		if (*it == state) {
			break;
		}
	}
	return *it;
}
