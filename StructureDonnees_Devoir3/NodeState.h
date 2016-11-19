#pragma once
#include <memory>
using namespace std;

class State;
class NodeState
{
private:
	bool closed = false;
	State* predecessor = nullptr;
	int cost = 0;
public:

	explicit NodeState(bool closed = false, State* predecessor = nullptr, int cost = 0);
	NodeState(const NodeState& copy);
	virtual ~NodeState();

	bool isClosed() const;
	State* getPredecessor() const;
	int getCost() const;

	void setClosed(bool closed);
	void setPredecessor(State* predecessor);
	void setCost(int cost);

};
