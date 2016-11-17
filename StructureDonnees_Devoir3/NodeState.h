#pragma once
#include "State.h"

class NodeState
{
private:
	bool closed = false;
	shared_ptr<State> predecessor = nullptr;
	int cost = 0;
public:

	explicit NodeState(bool closed = false, shared_ptr<State> predecessor = nullptr, int cost = 0);
	NodeState(const NodeState& copy);

	bool isClosed() const;
	shared_ptr<State> getPredecessor() const;
	int getCost() const;

	void setClosed(bool closed);
	void setPredecessor(shared_ptr<State> predecessor);
	void setCost(int cost);

};
