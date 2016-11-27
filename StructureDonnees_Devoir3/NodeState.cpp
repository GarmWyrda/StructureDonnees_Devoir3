#include "stdafx.h"
#include "NodeState.h"
#include "State.h"

int NodeState::getCost() const
{
	return cost;
}

void NodeState::setClosed(bool closed)
{
	initialized = true;
	this->closed = closed;
}

void NodeState::setPredecessor(State* predecessor)
{
	initialized = true;
	this->predecessor = predecessor;
}

void NodeState::setCost(int cost)
{
	initialized = true;
	this->cost = cost;
}

State* NodeState::getPredecessor()
{
	return predecessor;
}


NodeState::NodeState()
{
	initialized = false;
	this->closed = false;
	this->predecessor = nullptr;
	this->cost = 0;
}

NodeState::~NodeState()
{
}

NodeState::NodeState(bool closed, State* predecessor, int cost)
{
	initialized = true;
	this->closed = closed;
	this->predecessor = predecessor;
	this->cost = cost;

}
void NodeState::setValues(bool closed, State* predecessor, int cost)
{
	initialized = true;
	this->closed = closed;
	this->predecessor = predecessor;
	this->cost = cost;
}

bool NodeState::isClosed() const
{
	return closed;
}

void NodeState::reset()
{
	initialized = false;
	this->closed = false;
	this->predecessor = nullptr;
	this->cost = 0;
}

bool NodeState::exists() const
{
	return initialized;
}
