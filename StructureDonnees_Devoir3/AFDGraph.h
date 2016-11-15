#pragma once
#include <memory>
#include "State.h"
using std::shared_ptr;

class AFDGraph
{
private :
	vector<State> states;
	int idStartState;
public:
	AFDGraph();
	virtual ~AFDGraph();
	void setStartState(int idStartState);
	void addNewState(int id, bool isfinal, bool start);
	void addState(State state, bool start);
	State &getState(int index);
	State &getStartState();
	State &getState(const State &state);
	const vector<State> &getStates() const;

};

