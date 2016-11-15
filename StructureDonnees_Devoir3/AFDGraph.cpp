#include "stdafx.h"
#include "AFDGraph.h"
#include <iostream>

using namespace std;


AFDGraph::AFDGraph()
{
	this->idStartState = -1;
	this->states = vector<State>();
}


AFDGraph::~AFDGraph()
{
}

void AFDGraph::setStartState(int idStartState)
{
	this->idStartState = idStartState;
}

void AFDGraph::addNewState(int id, bool isfinal, bool start)
{
	State newState = State(id, isfinal);
	this->states.push_back(newState);
	if (start) {
		this->idStartState = id;
	}
}

void AFDGraph::addState(State state, bool start)
{
	this->states.push_back(state);
	if (start) {
		this->idStartState = state.getId();
	}
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

State & AFDGraph::getStartState()
{
	vector<State>::iterator it;
	for (it = this->states.begin(); it != this->states.end(); ++it) {
		if (it->getId() == this->idStartState) {
			return *it;
		}
	}
	std::cout << "getStartState() not found" << std::endl;
	return State();
}

State & AFDGraph::getState(const State & state)
{
	vector<State>::iterator it;
	for (it = this->states.begin(); it != this->states.end(); ++it) {
		if (*it == state) {
			break;
		}
	}
	std::cout << "getState(const State & state) not found" << std::endl;
	return State();
}

const vector<State>& AFDGraph::getStates() const
{
	return this->states;
}
