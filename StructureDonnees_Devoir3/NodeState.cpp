#include "NodeState.h"

int NodeState::getCost() const
{
	return cost;
}

void NodeState::setClosed(bool closed)
{
	this->closed = closed;
}

void NodeState::setPredecessor(shared_ptr<State> predecessor)
{
	this->predecessor = predecessor;
}

void NodeState::setCost(int cost)
{
	this->cost = cost;
}

shared_ptr<State> NodeState::getPredecessor() const
{
	return predecessor;
}


NodeState::NodeState(const NodeState& copy)
{
	this->closed = copy.closed;
	this->predecessor = copy.predecessor;
	this->cost = copy.cost;

}

NodeState::NodeState(bool closed, shared_ptr<State> predecessor, int cost)
{
	this->closed = closed;
	this->predecessor = predecessor;
	this->cost = cost;
}

bool NodeState::isClosed() const
{
	return closed;
}
