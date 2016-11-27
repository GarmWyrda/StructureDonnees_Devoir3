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
	int getIdStart() const;
	void setStartState(int idStartState);
	void addNewState(int id, bool isfinal, bool start);
	void addState(State state, bool start);
	State &getState(int index);
	State &getStartState();
	const vector<State> &getStates() const;
	friend std::ostream& operator<< (std::ostream&, const AFDGraph);

};

