#include "stdafx.h"
#include "NodeState.h"
#include "State.h"

int NodeState::getCost() const
{
	return cost;
}

void NodeState::setClosed(bool closed)
{
	this->closed = closed;
}

void NodeState::setPredecessor(State* predecessor)
{
	this->predecessor = predecessor;
}

void NodeState::setCost(int cost)
{
	this->cost = cost;
}

State* NodeState::getPredecessor() const
{
	return predecessor;
}


NodeState::NodeState(const NodeState& copy)
{
	this->closed = copy.closed;
	this->predecessor = copy.predecessor;
	this->cost = copy.cost;

}

NodeState::~NodeState()
{
}

NodeState::NodeState(bool closed, State* predecessor, int cost)
{
	this->closed = closed;
	this->predecessor = predecessor;
	this->cost = cost;
}

bool NodeState::isClosed() const
{
	return closed;
}
