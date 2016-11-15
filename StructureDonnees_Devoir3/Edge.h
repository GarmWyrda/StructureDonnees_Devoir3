#pragma once
#include <string>
#include <memory>
using namespace std;
class State;

class Edge
{
private :
	shared_ptr<State> arrivalState;
	string transition;
	int weight;
public:
	Edge(shared_ptr<State> arrivalState, string transition, int weight);
	Edge(const Edge &edge);
	virtual ~Edge();
	shared_ptr<State> getArrivalState() const;
	string getTransition() const;
	int getWeight() const;
};

