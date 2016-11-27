#pragma once
#include <string>

using namespace std;
class State;

class Edge
{
private :
	State* arrivalState;
	string transition;
	int weight;
public:
	Edge(State* arrivalState, string transition, int weight);
	Edge(const Edge &edge);
	virtual ~Edge();
	State* getArrivalState() const;
	string getTransition() const;
	int getWeight() const;
};

