#pragma once
#include <vector>
#include "Edge.h"
#include "NodeState.h"
using std::vector;

class State
{
private :
	int id;
	bool isfinal;
	vector<Edge> transitions;
	shared_ptr<NodeState> nodeSate = nullptr;

public:
	State();
	State(int id, bool isfinal, vector<Edge> transitions);
	State(int id, bool isfinal);
	virtual ~State();
	int getId() const;
	bool getFinal() const;
	void setFinal(bool newFinal);
	size_t getNbTransitions() const;
	vector<Edge> getTransitions() const;
	Edge getEdge(int i);
	void addTransition(Edge newEdge);
	void addTransition(shared_ptr<State> outState, string transition, int weight);
	void setNodeState(shared_ptr<NodeState> nodeState);
	shared_ptr<NodeState> getNodeState() const;

	bool operator<(const State state) const;
	bool operator>(const State state) const;
	bool operator==(const State state) const;
	bool operator!=(const State state) const;
};

