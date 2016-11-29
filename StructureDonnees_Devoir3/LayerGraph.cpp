#include "stdafx.h"
#include "LayerGraph.h"
#include <algorithm>
#include "Path.h"
using namespace std;

LayerGraph::LayerGraph(AFDGraph graph, int wordLength, vector<int> minValues, vector<int> maxValues)
{
	//Limits
	this->minValues = minValues;
	this->maxValues = maxValues;

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
	State* arrivalFromSource = &this->layers[0][statePos];
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
				State* arrivalState = &this->layers[i][statePos];
				state.addTransition(arrivalState, edge.getTransition(), edge.getWeight());
			}
		}
	}

	//Destination
	this->destination = State(-2, true);

	for (State& state : this->layers[this->layers.size() - 1]) {
		if (state.getFinal()) {
			State* destination = &this->destination;
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


bool _compareCosts(State* xState, State* yState)
{
	return xState->getNodeState().getCost() > yState->getNodeState().getCost();
}

vector<State> LayerGraph::findShortestPath()
{
	_resetNodesState(&source);

	//Dijkstra
	//Propagate states
	vector<State*> heap = vector<State*>();
	int trialCost = 0;


	source.getNodeState().setValues(false, nullptr, 0);

	State* currentState = &source;
	do {
		currentState->getNodeState().setClosed(true);
		for (Edge transition : currentState->getTransitions())
		{
			trialCost = currentState->getNodeState().getCost() + transition.getWeight();
			State* arrivalState = transition.getArrivalState();
			if (!arrivalState->getNodeState().exists())
			{

				arrivalState->getNodeState().setValues(false, currentState, trialCost);
				heap.push_back(arrivalState);
				sort(heap.begin(), heap.end(), &_compareCosts);
			}
			else
			{
				if (!arrivalState->getNodeState().isClosed() && trialCost < arrivalState->getNodeState().getCost())
				{
					int i = find(heap.begin(), heap.end(), arrivalState) - heap.begin();
					heap[i]->getNodeState().setCost(trialCost);
					arrivalState->getNodeState().setPredecessor(currentState);
					arrivalState->getNodeState().setCost(trialCost);
				}
			}
		}

		currentState = heap.back();
		heap.pop_back();

	} while (currentState->getId() != destination.getId());

	//Build optimal path
	if (currentState->getId() == destination.getId())
	{
		vector<State> path = vector<State>();
		State* predecessor = currentState;
		while (predecessor != nullptr)
		{
			path.push_back(*predecessor);
			predecessor = predecessor->getNodeState().getPredecessor();
		}
		return path;
	}
	else
	{
		return vector<State>();
	}
}


bool LayerGraph::_isValidPath(Path path)
{
	State* lastState = path.getPath()[0];
	if (lastState->getId() == destination.getId())
	{
		for (size_t i = 0; i < path.getLettersCount().size(); i++) {
			if (path.getLettersCount()[i] < minValues[i] || path.getLettersCount()[i] > maxValues[i]) {
				return false;
			}
		}
		return true;
	} else
	{
		for (size_t i = 0; i < path.getLettersCount().size(); i++) {
			if (path.getLettersCount()[i] > maxValues[i]) {
				return false;
			}
		}

		int missingTransitions = 0;
		for (size_t i = 0; i < path.getLettersCount().size(); i++) {
			if (path.getLettersCount()[i] < minValues[i]) {
				missingTransitions += minValues[i] - path.getLettersCount()[i];
			}
		}

		if (missingTransitions > layers.size() - path.getPath().size() + 1) {
			return false;
		}

		bool isValid = false;
		for (Edge transition : lastState->getTransitions()) {
			Path nextPath = Path(path);
			nextPath.addFrontState(transition.getArrivalState());
			if (_isValidPath(nextPath)) {
				isValid = true;
				break;
			}
		}
		return isValid;
	}

}

vector<State> LayerGraph::_findShortestPathWithLimitsAndGoal(State* goalState)
{
	//Propagate states
	vector<State*> heap = vector<State*>();
	int trialCost = 0;

	source.getNodeState().setValues(false, nullptr, 0);

	State* currentState = &source;
	heap.push_back(&source);

	while (!heap.empty() && currentState != goalState)
	{
		currentState = heap.back();
		heap.pop_back();

		currentState->getNodeState().setClosed(true);
		for (Edge transition : currentState->getTransitions())
		{
			trialCost = currentState->getNodeState().getCost() + transition.getWeight();
			State* arrivalState = transition.getArrivalState();
			if (!arrivalState->getNodeState().exists())
			{
				Path path = Path(maxValues.size());
				path.addState(arrivalState);
				State* copyState = currentState;
				while (copyState->getId() != source.getId())
				{
					path.addState(copyState);
					copyState = copyState->getNodeState().getPredecessor();
				}
				path.addState(&source);
				if (_isValidPath(path))
				{
					arrivalState->getNodeState().setValues(false, currentState, trialCost);
					heap.push_back(arrivalState);
					sort(heap.begin(), heap.end(), &_compareCosts);
				}

			}
			else
			{
				if (!arrivalState->getNodeState().isClosed() && trialCost < arrivalState->getNodeState().getCost())
				{
					int i = find(heap.begin(), heap.end(), arrivalState) - heap.begin();
					heap[i]->getNodeState().setCost(trialCost);
					arrivalState->getNodeState().setPredecessor(currentState);
					arrivalState->getNodeState().setCost(trialCost);
				}
			}
		}


	}

	//Build optimal path
	if (currentState->getId() == goalState->getId())
	{
		vector<State> path = vector<State>();
		State* predecessor = currentState;
		while (predecessor != nullptr)
		{
			path.push_back(*predecessor);
			predecessor = predecessor->getNodeState().getPredecessor();
		}
		return path;
	}
	else
	{
		return vector<State>();
	}
}

vector<State> LayerGraph::findShortestPathWithLimits()
{
	_resetNodesState(&source);

	//Propagate states
	vector<State*> heap = vector<State*>();
	int trialCost = 0;

	source.getNodeState().setValues(false, nullptr, 0);

	State* currentState = nullptr;
	heap.push_back(&source);

	while (!heap.empty() && (currentState == nullptr || currentState->getId() != destination.getId()))
	{
		currentState = heap.back();
		heap.pop_back();

		currentState->getNodeState().setClosed(true);
		for (Edge& transition : currentState->getTransitions())
		{
			trialCost = currentState->getNodeState().getCost() + transition.getWeight();
			State* arrivalState = transition.getArrivalState();
			if (!arrivalState->getNodeState().exists())
			{
				Path path = Path(maxValues.size());
				path.addState(arrivalState);
				State* copyState = currentState;
				while (copyState->getId() != source.getId())
				{
					path.addState(copyState);
					copyState = copyState->getNodeState().getPredecessor();
				}
				path.addState(&source);
				if (_isValidPath(path))
				{
					transition.getArrivalState()->getNodeState().setValues(false, currentState, trialCost);
					heap.push_back(arrivalState);
					sort(heap.begin(), heap.end(), &_compareCosts);
				}

			}
			else
			{
				if (!arrivalState->getNodeState().isClosed() && trialCost < arrivalState->getNodeState().getCost())
				{
					int i = find(heap.begin(), heap.end(), arrivalState) - heap.begin();
					heap[i]->getNodeState().setCost(trialCost);
					arrivalState->getNodeState().setPredecessor(currentState);
					arrivalState->getNodeState().setCost(trialCost);
				}
			}
		}


	}

	//Build optimal path
	if (currentState->getId() == destination.getId())
	{
		vector<State> path = vector<State>();
		State* predecessor = currentState;
		while (predecessor != nullptr)
		{
			path.push_back(*predecessor);
			predecessor = predecessor->getNodeState().getPredecessor();
		}
		return path;
	}
	else
	{
		return vector<State>();
	}
}

vector<State> LayerGraph::getEdgeOnValidPath(Edge* edge)
{
	_resetNodesState(&source);

	vector<State> tempPath = _findShortestPathWithLimitsAndGoal(edge->getArrivalState());

	//find first valid path with cost

	Path path = Path(maxValues.size());
	for (State& state : tempPath)
	{
		path.addState(&state);
	}

	Path path2 = _getFirstValidPath(path);
	tempPath = path2.toVector();
	return tempPath;
}

vector<State> LayerGraph::getEdgeOnValidPathWithCost(Edge* edge, int cost)
{
	_resetNodesState(&source);
	vector<State> tempPath = _findShortestPathWithLimitsAndGoal(edge->getArrivalState());

	//find first valid path with cost

	Path path = Path(maxValues.size());
	for (State& state : tempPath)
	{
		path.addState(&state);
	}

	path = _getFirstValidPath(path, cost);
	vector<State> returnVector = vector<State>();
	for (State* state : path.getPath())
	{
		returnVector.push_back(*state);
	}
	return returnVector;

}

Path LayerGraph::_getFirstValidPath(Path path)
{
	State* lastState = path.getPath()[0];
	if (lastState->getId() == destination.getId())
	{
		for (size_t i = 0; i < path.getLettersCount().size(); i++) {
			if (path.getLettersCount()[i] < minValues[i] || path.getLettersCount()[i] > maxValues[i]) {
				return Path(maxValues.size());
			}
		}
		return path;
	}
	else {

		for (int i = 0; i < path.getLettersCount().size(); i++) {
			if (path.getLettersCount()[i] > maxValues[i]) {
				return Path(maxValues.size());
			}
		}

		int nbMissingEdges = 0;
		for (int i = 0; i < path.getLettersCount().size(); i++) {
			if (path.getLettersCount()[i] < minValues[i]) {
				nbMissingEdges += minValues[i] - path.getLettersCount()[i];
			}
		}

		if (nbMissingEdges > layers.size() - path.getPath().size() + 1) {
			return Path(maxValues.size());
		}

		for (Edge edge : lastState->getTransitions()) {
			Path nextPath = Path(path);
			nextPath.addFrontState(edge.getArrivalState());
			Path returnPath = _getFirstValidPath(nextPath);
			if (returnPath.getPath().size() > 0) {
				return returnPath;
			}

		}
		return Path(maxValues.size());
	}
}

Path LayerGraph::_getFirstValidPath(Path path, int cost)
{
	State* lastState = path.getPath()[0];
	if (lastState->getId() == destination.getId())
	{
		for (size_t i = 0; i < path.getLettersCount().size(); i++) {
			if (path.getLettersCount()[i] < minValues[i] || path.getLettersCount()[i] > maxValues[i]) {
				return Path(maxValues.size());
			}
		}
		return path;
	}
	else {

		for (int i = 0; i < path.getLettersCount().size(); i++) {
			if (path.getLettersCount()[i] > maxValues[i]) {
				return Path(maxValues.size());
			}
		}

		int nbMissingEdges = 0;
		for (int i = 0; i < path.getLettersCount().size(); i++) {
			if (path.getLettersCount()[i] < minValues[i]) {
				nbMissingEdges += minValues[i] - path.getLettersCount()[i];
			}
		}

		if (nbMissingEdges > layers.size() - path.getPath().size() + 1) {
			return Path(maxValues.size());
		}

		for (Edge edge : lastState->getTransitions()) {
			Path nextPath = Path(path);
			nextPath.addFrontState(edge.getArrivalState());
			if (nextPath.getCost() <= cost) {
				Path returnPath = _getFirstValidPath(nextPath, cost);
				if (returnPath.getPath().size() > 0) {
					return returnPath;
				}
			}
		}
		return Path(maxValues.size());
	}
}

Edge* LayerGraph::getEdge(State* state1, State* state2)
{
	for (Edge& edge : state1->getTransitions()) {
		if (edge.getArrivalState()->getId() == state2->getId()) {
			return &edge;
		}
	}
	return nullptr;
}

State* LayerGraph::getState(int nodeId, int layer)
{
	if (nodeId == -1)
		return &source;
	else if (nodeId == -2)
		return &destination;
	else {
		for (State& state : layers[layer]) {
			if (nodeId == state.getId()) {
				return &state;
			}
		}
	}
}

int LayerGraph::findShortestPathWithLimitsOld(vector<State*> &returnPath)
{

	vector<int> lettersCount = vector<int>();
	for (size_t i = 0; i < minValues.size(); i++) lettersCount.push_back(0);
	vector<vector<State*>> paths = vector<vector<State*>>();

	for (size_t i = 0; i < source.getTransitions().size(); i++)
	{
		State* arrivalState = source.getTransitions()[i].getArrivalState();
		vector<State*> currentPath = vector<State*>();
		currentPath.push_back(arrivalState);
		_buildPaths(arrivalState, paths, currentPath, lettersCount);
	}


	int shorttestPathIndex = 0;
	int shortestCost = INT_MAX;
	for (size_t j = 0; j < paths.size();j++)
	{
		vector<State*> path = paths[j];
		int cost = 0;
		for (size_t i = 0; i < path.size() - 1; i++)
		{
			State* state = path[i];
			int arrivalId = path[i + 1]->getId();
			vector<Edge> transitions = path[i]->getTransitions();
			vector<Edge>::iterator itTransition = find_if(transitions.begin(), transitions.end(), [arrivalId](const Edge& transition) {return transition.getArrivalState()->getId() == arrivalId;});
			cost = cost + itTransition->getWeight();
		}

		if (cost < shortestCost) {
			shortestCost = cost;
			shorttestPathIndex = j;
		}
	}
	returnPath = paths[shorttestPathIndex];
	return shortestCost;


}

void LayerGraph::_buildPaths(State* currenState, vector<vector<State*>>& paths, vector<State*> currentPath, vector<int> lettersCount)
{
	for (size_t i = 0; i < currenState->getTransitions().size(); i++)
	{
		Edge transition = currenState->getTransitions()[i];
		if (transition.getArrivalState()->getId() != destination.getId())
		{
			int letterRead = stoi(transition.getTransition()) - 1;
			lettersCount[letterRead]++;
			if (lettersCount[letterRead] <= maxValues[letterRead])
			{
				currentPath.push_back(transition.getArrivalState());
				_buildPaths(transition.getArrivalState(), paths, currentPath, lettersCount);
				currentPath.pop_back();

			}
			lettersCount[letterRead]--;
		}
		else
		{
			bool validPath = true;
			for (size_t j = 0; j < lettersCount.size(); j++)
			{
				if (lettersCount[j] < minValues[j] || lettersCount[j] > maxValues[j])
				{
					validPath = false;
					break;
				}
			}
			if (validPath) paths.push_back(currentPath);

		}
	}

}

void LayerGraph::_resetNodesState(State* currentState)
{
	currentState->getNodeState().reset();
	for (Edge& transition : currentState->getTransitions()) {
		if(transition.getArrivalState()->getNodeState().exists())
		{
			_resetNodesState(transition.getArrivalState());
		}
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
