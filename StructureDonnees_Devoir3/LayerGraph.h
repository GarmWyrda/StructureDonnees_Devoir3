#pragma once
#include "State.h"
#include "AFDGraph.h"
#include "Path.h"

class LayerGraph
{
private:
	State source;
	State destination;
	vector<vector<State>> layers;
	vector<int> minValues;
	vector<int> maxValues;

	bool _isValidPath(Path path);
	vector<State> _findShortestPathWithLimitsAndGoal(State* goalState);
	Path _getFirstValidPath(Path path);
	Path _getFirstValidPath(Path path, int cost);
	
	void _buildPaths(State* currenState, vector<vector<State*>>& paths, vector<State*> currentPath, vector<int> letterCount);
	void _resetNodesState(State* currentState);
public:
	LayerGraph(AFDGraph graph, int wordLength, vector<int> minValues, vector<int> maxValues);
	~LayerGraph();
	State getSource() const;

	vector<State> findShortestPath();
	vector<State> findShortestPathWithLimits();

	vector<State> getEdgeOnValidPath(Edge* edge);
	vector<State> getEdgeOnValidPathWithCost(Edge* edge, int cost);

	int findShortestPathWithLimitsOld(vector<State*> &returnPath);

	State* getState(int id, int layer);
	Edge* getEdge(State* state1, State* state2);

	friend ostream& operator<< (ostream&, const LayerGraph);
};

