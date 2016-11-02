#pragma once
#include <memory>
#include "State.h"
using std::shared_ptr;

class AFDGraph
{
private :
	vector<State> states;
	State initState;
public:
	AFDGraph();
	~AFDGraph();
	State getInitState();

};

