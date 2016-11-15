#include "stdafx.h"
#include "LayerGraph.h"

LayerGraph::LayerGraph(AFDGraph graph, int wordLength)
{
	//Source
	vector<Edge> edges = vector<Edge>();
	Edge edge = Edge(&graph.getStartState(), "", 0);
	edges.push_back(edge);
	this->source = State(-1, false, edges);

	//Layers
	this->layers = vector<vector<State>>();
	this->layers.push_back(vector<State>());
	for (State state : graph.getStates()) {
		State newState = State(state.getId(), state.getFinal());
		this->layers[0].push_back(newState);
	}

	for (int i = 1; i < wordLength; i++) {
		this->layers.push_back(vector<State>());
		for (State state : graph.getStates()) {
			State newState = State(state.getId(), state.getFinal());
			this->layers[0].push_back(newState);
		}

		for (State state : this->layers[i - 1]) {
			State stateInAFDGraph = graph.getState(state.getId());
			for (Edge edge : stateInAFDGraph.getTransitions()) {
				state.addTransition(edge.getArrivalState(), edge.getTransition(), edge.getWeight());
			}
		}
	}

	//Destination
	this->destination = State(-2, true);

	for (State state : this->layers[this->layers.size() - 1]) {
		if (state.getFinal()) {
			Edge newEdge = Edge(&this->destination, "", 0);
			state.addTransition(newEdge);
		}
	}

}

LayerGraph::~LayerGraph()
{
}

vector<State> LayerGraph::findShortestPath()
{
	bool goalReached = _propagateStates(source, destination);
	if (goalReached)
		//return buildOptimalPath(source, destination);
}


bool LayerGraph::_propagateStates(State startState, State goalState)
{
	vector<State> heap = vector<State>();

	State currentState = startState;

	do {
		vector<shared_ptr<State>> neighbors = vector<shared_ptr<State>>();
		for (Edge transition : currentState.getTransitions())
		{
			neighbors.push_back(transition.getArrivalState());
		}
	} while (true);
	
	return false;
}
