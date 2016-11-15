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

public:
	State(int id, bool isfinal, vector<Edge> transitions);
	State(int id, bool isfinal);
	virtual ~State();
	int getId() const;
	bool getFinal();
	void setFinal(bool newFinal);
	int getNbTransitions();
	vector<Edge> getTransitions();
	Edge getEdge(int i);
	void addTransition(Edge newEdge);
	void addTransition(State* outState, string transition, int weight);
};

bool const operator==(State const &state, State const &otherState);

