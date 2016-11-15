#include "stdafx.h"
#include "Edge.h"
#include "State.h"



Edge::Edge(shared_ptr<State> arrivalState, string transition, int weight) : arrivalState(arrivalState)
{
	this->arrivalState = arrivalState;
	this->transition = transition;
	this->weight = weight;
}

Edge::Edge(const Edge & edge) : arrivalState(arrivalState)
{
	this->arrivalState = edge.getArrivalState();
	this->transition = edge.getTransition();
	this->weight = edge.getWeight();
}


Edge::~Edge()
{
}

shared_ptr<State> Edge::getArrivalState() const
{
	return this->arrivalState;
}

string Edge::getTransition() const
{
	return this->transition;
}


int Edge::getWeight() const
{
	return this->weight;
}
