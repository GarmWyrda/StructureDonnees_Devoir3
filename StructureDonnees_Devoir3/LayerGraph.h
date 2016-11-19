#pragma once
#include "State.h"
#include "AFDGraph.h"

class LayerGraph
{
private:
	State source;
	State destination;
	vector<vector<State>> layers;
	vector<State> _djiskta(State startState, State goalState);

public:
	LayerGraph(AFDGraph graph, int wordLength);
	~LayerGraph();
	State getSource() const;
	vector<State> findShortestPath();
	friend std::ostream& operator<< (std::ostream&, const LayerGraph);
};

