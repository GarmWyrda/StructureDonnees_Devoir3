#include "stdafx.h"
#include "LayerGraph.h"

LayerGraph::LayerGraph(AFDGraph graph, int wordLength)
{
	
	//Layers
	this->layers = vector<vector<State>>();
	this->layers.push_back(vector<State>());
	for (State state : graph.getStates()) {
		State newState = State(state.getId(), state.getFinal());
		this->layers[0].push_back(newState);
	}

	//Source
	//Create Edge From Source To Start State of AFDGrpah (first Layer)
	vector<Edge> edges = vector<Edge>();
	int idArrivalFromSource = graph.getStartState().getId();
	bool find = false;
	State arrivalState = State();
	while (!find) {
		vector<State>::iterator it;
		for (it = this->layers[i].begin(); it != this->layers[i].end(); ++it) {
			if (it->getId() == idArrivalFromSource) {
				break;
			}
		}
		arrivalState = *it;
		find = true;
	}
	Edge edge = Edge(make_shared<State>(arrivalState), "", 0);
	edges.push_back(edge);
	this->source = State(-1, false, edges);

	for (int i = 1; i < wordLength; i++) {
		this->layers.push_back(vector<State>());
		for (State state : graph.getStates()) {
			State newState = State(state.getId(), state.getFinal());
			this->layers[0].push_back(newState);
		}


		//Create Edges for each layer
		for (State state : this->layers[i - 1]) {
			State stateInAFDGraph = graph.getState(state.getId());
			for (Edge edge : stateInAFDGraph.getTransitions()) {
				int idArrivalState = edge.getArrivalState()->getId();
				bool find = false;
				State arrivalState = State();
				while (!find) {
					vector<State>::iterator it;
					for (it = this->layers[i].begin(); it != this->layers[i].end(); ++it) {
						if (it->getId() == idArrivalState) {
							break;
						}
					}
					 arrivalState = *it;
					 find = true;
				}
				state.addTransition(make_shared<State>(arrivalState), edge.getTransition(), edge.getWeight());
			}
		}
	}

	//Destination
	this->destination = State(-2, true);

	for (State state : this->layers[this->layers.size() - 1]) {
		if (state.getFinal()) {
			Edge newEdge = Edge(make_shared<State>(this->destination), "", 0);
			state.addTransition(newEdge);
		}
	}

}

LayerGraph::~LayerGraph()
{
}

vector<State> LayerGraph::findShortestPath()
{
	bool goalReached = _propagateStates(source, destination, 0);
	//if (goalReached)
		//return buildOptimalPath(source, destination);
}


bool LayerGraph::_propagateStates(State startState, State goalState, int sumCost)
{
	vector<State> heap = vector<State>();
	int trialCost = 0;
	State currentState = startState;

	do {
		currentState.setClosed(true);
		for (Edge transition : currentState.getTransitions())
		{
			trialCost = sumCost + transition.getWeight();
			if(!transition.getArrivalState()->isClosed())
			{
				
			}
		}

	} while (true);
	
	return false;
}
