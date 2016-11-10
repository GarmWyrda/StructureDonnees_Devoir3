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
	void addNewState(int id, bool final, bool start);
	const State &getState(int index) const;
	State &getState(int id);
	const State &getState(const State &state) const;

};

