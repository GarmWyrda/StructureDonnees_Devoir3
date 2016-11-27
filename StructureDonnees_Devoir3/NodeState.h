#pragma once

class State;
class NodeState
{
private:
	bool initialized;
	bool closed;
	State* predecessor;
	int cost;
public:
	NodeState();
	NodeState(bool closed, State * predecessor, int cost);
	virtual ~NodeState();

	bool isClosed() const;
	void reset();
	bool exists() const;
	State* getPredecessor();
	int getCost() const;

	void setValues(bool closed, State * predecessor, int cost);
	void setClosed(bool closed);
	void setPredecessor(State* predecessor);
	void setCost(int cost);

};
