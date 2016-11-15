#pragma once
#include "State.h"
#include "AFDGraph.h"

class LayerGraph
{
private:
	State source;
	State destination;
	vector<vector<State>> layers;
	bool _propagateStates(State startNode, State goalNode);

public:
	LayerGraph(AFDGraph graph, int wordLength);
	~LayerGraph();
	vector<State> findShortestPath();
};

