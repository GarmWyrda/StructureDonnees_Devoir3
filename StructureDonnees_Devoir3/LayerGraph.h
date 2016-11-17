#pragma once
#include "State.h"
#include "AFDGraph.h"

class LayerGraph
{
private:
	State source;
	State destination;
	vector<vector<State>> layers;
	bool _propagateStates(State startSate, State goalState, int sumCost);
	vector<State> _buildOptimalPath(State startSate, State goalState) const;

public:
	LayerGraph(AFDGraph graph, int wordLength);
	~LayerGraph();
	vector<State> findShortestPath();
};

