#pragma once
#include <memory>
using std::shared_ptr;
class State;

class Edge
{
private :
	shared_ptr<State> arrivalState;
	char transition;
	int weight;
public:
	Edge();
	~Edge();
};

