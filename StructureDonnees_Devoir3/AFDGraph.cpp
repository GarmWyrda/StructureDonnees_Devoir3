#include "stdafx.h"
#include "AFDGraph.h"
#include <iostream>
#include "NotFoundException.h"
#include <algorithm>

using namespace std;


AFDGraph::AFDGraph()
{
	this->idStartState = -1;
	this->states = vector<State>();
}


AFDGraph::~AFDGraph()
{
}

int AFDGraph::getIdStart() const
{
	return this->idStartState;
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
	/*
	vector<State>::iterator it;
	for (it = this->states.begin(); it != this->states.end(); ++it) {
		if (it->getId() == id) {
			break;
		}
	}
	return *it;
	*/
	int statePos = find_if(this->states.begin(), this->states.end(), [id](const State& state) {return state.getId() == id; }) - this->states.begin();
	return this->states[statePos];
}

State & AFDGraph::getStartState()
{
	/*
	vector<State>::iterator it;
	for (it = this->states.begin(); it != this->states.end(); ++it) {
		if (it->getId() == this->idStartState) {
			return *it;
		}
	}
	throw NotFoundException();
	*/
	int idToFind = this->idStartState;
	int statePos = find_if(this->states.begin(), this->states.end(), [idToFind](const State& state) {return state.getId() == idToFind; }) - this->states.begin();
	return this->states[statePos];
}

State & AFDGraph::getState(const State & state)
{
	/*
	vector<State>::iterator it;
	for (it = this->states.begin(); it != this->states.end(); ++it) {
		if (it->getId() == state.getId()) {
			break;
		}
	}
	throw NotFoundException();
	*/
	int idToFind = state.getId();
	int statePos = find_if(this->states.begin(), this->states.end(), [idToFind](const State& findState) {return findState.getId() == idToFind; }) - this->states.begin();
	return this->states[statePos];
}

const vector<State>& AFDGraph::getStates() const
{
	return this->states;
}

std::ostream & operator<<(std::ostream & stream, const AFDGraph afdGraph)
{
	stream << "this is the ID of the starting State : " << endl;
	stream << afdGraph.getIdStart() << endl;

	stream << "each line represents a state and all of his direct neighbors" << endl;
	for (State state : afdGraph.getStates()) {
		stream << state.getId() << "---> ";
		for (Edge edge : state.getTransitions()) {
			stream << edge.getArrivalState()->getId();
			stream << " and ";
		}
		stream << endl;
	}
	return stream;
}
