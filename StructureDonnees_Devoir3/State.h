#pragma once
#include <memory>
#include <vector>
using std::shared_ptr;
using std::vector;
class Edge;

class State
{
private :
	int id;
	bool final;
	vector<Edge> transitions;

public:
	State();
	State(int id, bool final, vector<Edge> transitions);
	~State();
	int getId();
	void setId(int newId);
	bool getFinal();
	void setFinal(bool newFinal);
	vector<Edge> getTransitions();
	void addTransition(Edge newEdge);
};

