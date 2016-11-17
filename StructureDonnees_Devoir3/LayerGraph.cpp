#include "stdafx.h"
#include "LayerGraph.h"
#include <algorithm>

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
	//Create Edge From Source To Start State of AFDGraph (first Layer)
	vector<Edge> edges = vector<Edge>();
	int idArrivalFromSource = graph.getStartState().getId();
	bool find = false;
	State arrivalState = State();
	while (!find) {
		vector<State>::iterator it;
		for (it = this->layers[0].begin(); it != this->layers[0].end(); ++it) {
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

State LayerGraph::getSource() const
{
	return this->source;
}

vector<State> LayerGraph::findShortestPath()
{
	bool goalReached = _propagateStates(source, destination, 0);
	if (goalReached)
		return _buildOptimalPath(source, destination);
	else
		return vector<State>();
}


bool LayerGraph::_propagateStates(State startState, State goalState, int sumCost)
{
	vector<State> heap = vector<State>();
	make_heap(heap.begin(), heap.end());
	int trialCost = 0;

	for(vector<State> layer : layers)
	{
		for(State state : layer)
		{
			state.setNodeState(nullptr);
		}
	}
	startState.setNodeState(make_shared<NodeState>(false, nullptr, 0));

	State currentState = startState;
	do {
		currentState.getNodeState()->setClosed(true);
		for (Edge transition : currentState.getTransitions())
		{
			trialCost = currentState.getNodeState()->getCost() + transition.getWeight();
			shared_ptr<State> arrivalState = transition.getArrivalState();
			if(arrivalState->getNodeState() == nullptr)
			{
				arrivalState->setNodeState(make_shared<NodeState>(false, make_shared<State>(currentState), trialCost));
				heap.push_back(*arrivalState);
				push_heap(heap.begin(), heap.end());
				sort_heap(heap.begin(), heap.end());
			} else
			{
				if(!arrivalState->getNodeState()->isClosed() && trialCost < arrivalState->getNodeState()->getCost())
				{
					int i = find(heap.begin(), heap.end(), *arrivalState) - heap.begin();
					heap[i].getNodeState()->setCost(trialCost);
					arrivalState->getNodeState()->setPredecessor(make_shared<State>(currentState));
					arrivalState->getNodeState()->setCost(trialCost);
				}
			}
		}
		
		pop_heap(heap.begin(), heap.end());
		currentState = heap.back();
		heap.pop_back();
		sort_heap(heap.begin(), heap.end());
	} while (!heap.empty() && currentState != goalState);
	
	return currentState == goalState;
}

vector<State> LayerGraph::_buildOptimalPath(State startState, State goalState) const
{
	vector<State> path = vector<State>();
	shared_ptr<State> predecessor = goalState.getNodeState()->getPredecessor();
	while(predecessor != nullptr)
	{
		path.push_back(*predecessor);
		predecessor = predecessor->getNodeState()->getPredecessor();
	}
	return path;
}

std::ostream & operator<<(std::ostream & stream, const LayerGraph layerGraph)
{
	Edge edge = layerGraph.getSource().getTransitions()[0];
	stream << "Source : " << endl;
	stream << edge.getArrivalState()->getId() << endl;

	stream << "---- Loop from Layer 0 ----" << endl;
	for (State state : layerGraph.layers[0]) {
		stream << state << endl;
		
	}

	return stream;
}
