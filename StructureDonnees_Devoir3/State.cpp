#include "stdafx.h"
#include "State.h"

State::State()
{
}

State::State(int id, bool isfinal, vector<Edge> transitions)
{
	this->isfinal = isfinal;
	this->id = id;
	this->transitions = transitions;
}

State::State(int id, bool isfinal)
{
	this->id = id;
	this->isfinal = isfinal;
	this->transitions = vector<Edge>();
}


State::~State()
{
}

int State::getId() const
{
	return this->id;
}


bool State::getFinal()
{
	return this->isfinal;
}

void State::setFinal(bool newFinal)
{
	this->isfinal = newFinal;
}

size_t State::getNbTransitions()
{
	return this->transitions.size();
}

vector<Edge> State::getTransitions()
{
	return this->transitions;
}

vector<Edge> State::getTransitions() const
{
	return this->transitions;
}

Edge State::getEdge(int i)
{
	return this->transitions[i];
}

void State::addTransition(Edge newEdge)
{
	this->transitions.push_back(newEdge);
}

void State::setClosed(bool isClosed)
{
	this->closed = isClosed;
}

void State::addTransition(shared_ptr<State> outState, string transition, int weight)
{
	Edge newEdge = Edge(outState, transition, weight);
	this->transitions.push_back(newEdge);
}

bool State::isClosed()
{
	return this->closed;
}

std::ostream & operator<<(std::ostream & stream, const State state)
{
	stream << state.getId() << "---> ";
	stream << " ( ";
	for (Edge edge : state.getTransitions()) {
		stream << edge.getArrivalState();
	}
	stream << " ) ";
}

bool const operator==(State const & state, State const & otherState)
{
	return state.getId() == otherState.getId();
}
