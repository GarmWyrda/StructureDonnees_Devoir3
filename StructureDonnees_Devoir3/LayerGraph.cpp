#include "stdafx.h"
#include "LayerGraph.h"
#include <algorithm>
#include <iostream>
using namespace std;

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
	int statePos = find_if(this->layers[0].begin(), this->layers[0].end(), [idArrivalFromSource](const State& state) {return state.getId() == idArrivalFromSource; }) - this->layers[0].begin();
	shared_ptr<State> arrivalFromSource(&this->layers[0][statePos]);
	Edge edge = Edge(arrivalFromSource, "", 0);
	edges.push_back(edge);
	this->source = State(-1, false, edges);

	for (int i = 1; i <= wordLength; i++) {
		this->layers.push_back(vector<State>());
		for (State state : graph.getStates()) {
			State newState = State(state.getId(), state.getFinal());
			this->layers[i].push_back(newState);
		}


		//Create Edges for each layer
		for (State& state : this->layers[i - 1]) {
			State stateInAFDGraph = graph.getState(state.getId());
			for (Edge edge : stateInAFDGraph.getTransitions()) {
				int idArrivalState = edge.getArrivalState()->getId();
				int statePos = find_if(this->layers[i].begin(), this->layers[i].end(), [&idArrivalState, i](const State& state) {return state.getId() == idArrivalState;}) - this->layers[i].begin();
				shared_ptr<State> arrivalState(&this->layers[i][statePos]);
				state.addTransition(arrivalState, edge.getTransition(), edge.getWeight());
			}
		}
	}

	//Destination
	this->destination = State(-2, true);

	for (State& state : this->layers[this->layers.size() - 1]) {
		if (state.getFinal()) {
			shared_ptr<State> destination(&this->destination);
			Edge newEdge = Edge(destination, "", 0);
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

		return _djiskta(source, destination);
}
bool _compareCosts(const shared_ptr<State>xState, const shared_ptr<State> yState)
{
	return xState->getNodeState()->getCost() > yState->getNodeState()->getCost();
}


vector<State> LayerGraph::_djiskta(State startState, State goalState)
{
	//Propagate states
	vector<shared_ptr<State>> heap = vector<shared_ptr<State>>();
	int trialCost = 0;

	for (vector<State> layer : layers)
	{
		for (State state : layer)
		{
			state.setNodeState(nullptr);
		}
	}
	startState.setNodeState(make_shared<NodeState>(false, nullptr, 0));

	shared_ptr<State> currentState = make_shared<State>(startState);
	do {
		currentState->getNodeState()->setClosed(true);
		for (Edge transition : currentState->getTransitions())
		{
			trialCost = currentState->getNodeState()->getCost() + transition.getWeight();
			shared_ptr<State> arrivalState = transition.getArrivalState();
			if (arrivalState->getNodeState() == nullptr)
			{

				arrivalState->setNodeState(make_shared<NodeState>(false, currentState, trialCost));
				heap.push_back(arrivalState);
				sort(heap.begin(), heap.end(), &_compareCosts);
			}
			else
			{
				if (!arrivalState->getNodeState()->isClosed() && trialCost < arrivalState->getNodeState()->getCost())
				{
					int i = find(heap.begin(), heap.end(), arrivalState) - heap.begin();
					heap[i]->getNodeState()->setCost(trialCost);
					arrivalState->getNodeState()->setPredecessor(currentState);
					arrivalState->getNodeState()->setCost(trialCost);
				}
			}
		}

		currentState = heap.back();
		heap.pop_back();

	} while (currentState->getId() != goalState.getId());

	//Build optimal path
	if(currentState->getId() == goalState.getId())
	{
		vector<State> path = vector<State>();
		shared_ptr<State> predecessor = currentState;
		while (predecessor != nullptr)
		{
			path.push_back(*predecessor);
			predecessor = predecessor->getNodeState()->getPredecessor();
			//Edge transition = find_if(predecessor->getTransitions().begin(), predecessor->getTransitions().end(), [&predecessor](const Edge& transition){ transition.getArrivalState()->getId() == predecessor->getId()} )
		}
		return path;
	} else
	{
		return vector<State>();
	}
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
