#include "stdafx.h"
#include "Edge.h"


Edge::Edge()
{
}

Edge::Edge(shared_ptr<State> arrivalState, char transition, int weight)
{
	this->arrivalState = arrivalState;
	this->transition = transition;
	this->weight = weight;
}


Edge::~Edge()
{
}

shared_ptr<State> Edge::getArrivalState()
{
	return this->arrivalState;
}

void Edge::setArrivalState(shared_ptr<State> newState)
{
	this->arrivalState = newState;
}

char Edge::getTransition()
{
	return this->transition;
}

void Edge::setTransition(char newTransition)
{
	this->transition = newTransition;
}
