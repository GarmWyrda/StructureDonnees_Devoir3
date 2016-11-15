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
	const State &getState(int index) const;
	State &getState(int id);
	const State &getState(const State &state) const;

};

