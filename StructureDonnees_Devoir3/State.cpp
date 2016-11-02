#include "stdafx.h"
#include "State.h"

State::State()
{
}

State::State(int id, bool final, vector<Edge> transitions)
{
	this->final = final;
	this->id = id;
	this->transitions = transitions;
}


State::~State()
{
}

int State::getId()
{
	return this->id;
}

void State::setId(int newId)
{
	this->id = newId;
}

bool State::getFinal()
{
	return this->final;
}

void State::setFinal(bool newFinal)
{
	this->final = newFinal;
}

vector<Edge> State::getTransitions()
{
	return this->transitions;
}

void State::addTransition(Edge newEdge)
{
	this->transitions.push_back(newEdge);
}
