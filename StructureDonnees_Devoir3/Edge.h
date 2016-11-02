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
	Edge(shared_ptr<State> arrivalState, char transition, int weight);
	~Edge();
	shared_ptr<State> getArrivalState();
	void setArrivalState(shared_ptr<State> newState);
	char getTransition();
	void setTransition(char newTransition);
};

