#pragma once
#include <vector>
#include "Edge.h"
using std::vector;

class State
{
private :
	int id;
	bool isfinal;
	vector<Edge> transitions;
	bool closed = false;

public:
	State();
	State(int id, bool isfinal, vector<Edge> transitions);
	State(int id, bool isfinal);
	virtual ~State();
	int getId() const;
	bool getFinal();
	void setFinal(bool newFinal);
	size_t getNbTransitions();
	vector<Edge> getTransitions();
	Edge getEdge(int i);
	void addTransition(Edge newEdge);
	void addTransition(shared_ptr<State> outState, string transition, int weight);
	void setClosed(bool isClosed);
	bool isClosed();
};

bool const operator==(State const &state, State const &otherState);

