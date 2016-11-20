#pragma once
#include "State.h"
#include "AFDGraph.h"

class LayerGraph
{
private:
	State source;
	State destination;
	vector<vector<State>> layers;
	vector<int> minValues;
	vector<int> maxValues;
	vector<State> _dijkstra(State startState, State goalState);
	void _buildPaths(State* currenState, vector<vector<State*>>& paths, vector<State*> currentPath, vector<int> letterCount);
public:
	LayerGraph(AFDGraph graph, int wordLength, vector<int> minValues, vector<int> maxValues);
	~LayerGraph();
	State getSource() const;
	vector<State> findShortestPath();
	
	int findShortestPathWithLimits(vector<State*> &returnPath);
	friend ostream& operator<< (ostream&, const LayerGraph);
};

