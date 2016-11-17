// StructureDonnees_Devoir3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "State.h"
#include <algorithm>
#include "AFDGraph.h"
#include "LayerGraph.h"
using namespace std;


vector<string> split(const string &s, char delim) {
	vector<string> result = vector<string>();
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		result.push_back(item);
	}
	return result;
}

AFDGraph buildGraphTransitionFile(string fileName)
{
	ifstream infile(fileName);
	int alphabetSize;
	int nbStates;
	int initialState;
	int nbFinalStates;
	vector<string> finalStates = vector<string>();
	int nbTransition;
	string line;

	vector<vector<string>> transitionLines = vector<vector<string>>();

	getline(infile, line);
	alphabetSize = stoi(line);
	getline(infile, line);
	nbStates = stoi(line);
	getline(infile, line);
	initialState = stoi(line);
	getline(infile, line);
	nbFinalStates = stoi(line);

	getline(infile, line);
	finalStates = split(line, ' ');

	getline(infile, line);
	nbTransition = stoi(line);

	int numTransition = 0;

	while (infile && numTransition < nbTransition)
	{
		if (!getline(infile, line)) break;
		transitionLines.push_back(split(line, ' '));
		numTransition++;
	}
	
	vector<State> states = vector<State>();
	for(int i = 1; i <= nbStates; i++)
	{

		if (find(finalStates.begin(), finalStates.end(), to_string(i)) != finalStates.end())
			states.push_back(State(i, true));
		else
			states.push_back(State(i, false));
	}

	for (int i = 0; i < nbTransition; i++)
	{
		vector<State>::iterator itArrivalState = find_if(states.begin(), states.end(), [&transitionLines, i](const State& state) {return state.getId() == stoi(transitionLines[i][2]);});
		Edge transition = Edge(make_shared<State>(*itArrivalState), transitionLines[i][3], stoi(transitionLines[i][0]));
		vector<State>::iterator itStartState = find_if(states.begin(), states.end(), [&transitionLines, i](const State& state) {return state.getId() == stoi(transitionLines[i][1]);});
		itStartState->addTransition(transition);
	}

	AFDGraph graph = AFDGraph();
	for(State state : states)
	{
		if(state.getId() == initialState) graph.addState(state, true);
		else graph.addState(state, false);
	}

	return graph;


}

bool readLimitsFile(string fileName)
{
	ifstream infile(fileName);
	int alphabetSize;
	int wordSize;
	vector<string> minValues = vector<string>();
	vector<string> maxValues = vector<string>();
	string line;

	vector<vector<string>> transition = vector<vector<string>>();

	getline(infile, line);
	alphabetSize = stoi(line);
	getline(infile, line);
	wordSize = stoi(line);
	getline(infile, line);
	minValues = split(line, ' ');
	getline(infile, line);
	maxValues = split(line, ' ');

	return true;
}

int main()
{
	cout << "\t Plus court chemin avec contraintes pour graphe par couche\n" << endl;
	cout << "Vous devez avoir deux .afdC, le premier pour le graphe, le deuxième pour les bornes.\n" << endl;
	cout << "Entrez le nom du fichier contenant les transitions : " << endl;
	string transitionFilName;
	cin >> transitionFilName;
	bool fileFound = false;
	AFDGraph graph = AFDGraph();
	while (!fileFound)
	{
		try
		{
			AFDGraph graph = buildGraphTransitionFile(transitionFilName);
			fileFound = true;
		} catch(...)
		{
			cout << "\nErreur le fichier n'a pas été trouvé." << endl;
			cout << "Entrez le nom du fichier contenant les transitions : " << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> transitionFilName;
		}
		
	}

	cout << "Entrez le nom du fichier contenant les limites : " << endl;
	string limitsFileName;
	cin >> limitsFileName;
	fileFound = false;

	while (!fileFound)
	{
		try
		{
			readLimitsFile(limitsFileName);
			fileFound = true;
		}
		catch (...)
		{
			cout << "\nErreur le fichier n'a pas été trouvé." << endl;
			cout << "Entrez le nom du fichier contenant les transitions : " << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> limitsFileName;
		}

	}

	LayerGraph layergraph = LayerGraph(graph, 3);
	std::cout << layergraph << std::endl;

	system("pause");
	return 0;
}

