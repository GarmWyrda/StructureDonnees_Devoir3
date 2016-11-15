#pragma once
#include "State.h"
#include "AFDGraph.h"

class LayerGraph
{
private:
	State source;
	State destination;
	vector<vector<State>> layers;

public:
	LayerGraph(AFDGraph graph, int wordLength);
	~LayerGraph();
};

