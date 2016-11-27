#pragma once
#include <vector>
#include "Edge.h"
#include "NodeState.h"
using namespace std;


class State
{
private :
	int id;
	bool isfinal;
	vector<Edge> transitions;
	NodeState nodeSate;

public:
	State();
	State(int id, bool isfinal, vector<Edge> transitions);
	State(int id, bool isfinal);
	virtual ~State();
	int getId() const;
	bool getFinal() const;
	void setFinal(bool newFinal);
	size_t getNbTransitions() const;
	vector<Edge>& getTransitions();
	Edge& getEdge(int i);
	void addTransition(Edge newEdge);
	void addTransition(State* outState, string transition, int weight);
	NodeState& getNodeState();

	friend ostream& operator<< (ostream&, const State);
};

