#include "stdafx.h"
#include "State.h"

State::State(int id, bool final, vector<Edge> transitions)
{
	this->final = final;
	this->id = id;
	this->transitions = transitions;
}

State::State(int id, bool final)
{
	this->id = id;
	this->final = final;
	this->transitions = vector<Edge>();
}


State::~State()
{
}

int State::getId() const
{
	return this->id;
}


bool State::getFinal()
{
	return this->final;
}

void State::setFinal(bool newFinal)
{
	this->final = newFinal;
}

int State::getNbTransitions()
{
	return this->transitions.size();
}

vector<Edge> State::getTransitions()
{
	return this->transitions;
}

Edge State::getEdge(int i)
{
	return this->transitions[i];
}

void State::addTransition(Edge newEdge)
{
	this->transitions.push_back(newEdge);
}

void State::addTransition(State * outState, string transition, int weight)
{
	Edge newEdge = Edge(outState, transition, weight);
	this->transitions.push_back(newEdge);
}

bool const operator==(State const & state, State const & otherState)
{
	return state.getId() == otherState.getId();
}
