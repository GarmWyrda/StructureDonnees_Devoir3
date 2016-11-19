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
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
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

AFDGraph* buildGraphTransitionFile(string fileName)
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

	vector<State*> states = vector<State*>();
	for (int i = 1; i <= nbStates; i++)
	{

		if (find(finalStates.begin(), finalStates.end(), to_string(i)) != finalStates.end())
			states.push_back(new State(i, true));
		else
			states.push_back(new State(i, false));
	}

	for (int i = 0; i < nbTransition; i++)
	{
		vector<State*>::iterator itArrivalState = find_if(states.begin(), states.end(), [&transitionLines, i](const State* state) {return state->getId() == stoi(transitionLines[i][2]);});
		Edge transition = Edge(*itArrivalState, transitionLines[i][0], stoi(transitionLines[i][3]));
		vector<State*>::iterator itStartState = find_if(states.begin(), states.end(), [&transitionLines, i](const State* state) {return state->getId() == stoi(transitionLines[i][1]);});
		State* startState = *itStartState;
		startState->addTransition(transition);
	}

	AFDGraph* graph = new AFDGraph();
	for (State* state : states)
	{
		if (state->getId() == initialState) graph->addState(*state, true);
		else graph->addState(*state, false);
	}

	return graph;
}

LayerGraph buildLayerGraph(AFDGraph graph, string fileName)
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

	return LayerGraph(graph, wordSize);
}

void displayCommands()
{
	cout << "---------- Les commandes possibles sont les suivantes ----------\n" << endl;
	cout << "- help, ? : affiche la liste des commandes" << endl;
	cout << "- graphe : affiche à l'ecran les noeuds du graphe AFD" << endl;
	cout << "- lgraphe : affiche à l'ecran les noeuds du graphe par couche" << endl;
	cout << "- fichier : ecrit les noeuds du plus court chemin dans un fichier" << endl;
	cout << "- plus court chemin : calcule et affiche le plus court chemin" << endl;
	cout << "- recherche [lettre] : affiche les aretes lisant la lettre indiquee" << endl;
	cout << "- quitter : quitte le programme.\n" << endl;

}
void readCommand(AFDGraph graph, LayerGraph layerGraph) {

	cout << "commande : ";
	string command;
	getline(cin, command);

	if (command == "help" || command == "?")
	{
		displayCommands();

	}
	else if (command == "graphe")
	{
		cout << "\n" << graph << endl;
	}
	else if (command == "lgraphe")
	{
		cout << "\n" << layerGraph << endl;
	}
	else if (command == "fichier")
	{
		//NOT IMPLEMENTED YET
	}
	else if (command == "plus court chemin")
	{
		cout << "Les noeuds parcourus sont : ";
		vector<State> path = layerGraph.findShortestPath();
		for (size_t i = path.size() - 2; i > 0; i--)
		{
			cout << path[i].getId() << "-";
		}
		cout << "\nCout total : " << path.front().getNodeState()->getCost() << endl;
		cout << "Le mot construit est : ";
		for (size_t i = path.size() - 2; i > 0; i--)
		{
			int arrivalId = path[i-1].getId();
			vector<Edge> previousStateTransitions = path[i].getTransitions();
			vector<Edge>::iterator itTransition = find_if(previousStateTransitions.begin(), previousStateTransitions.end(), [arrivalId](const Edge& transition) {return transition.getArrivalState()->getId() == arrivalId;});
			cout << itTransition->getTransition();
		}
		cout << endl;

	}
	else if (strncmp(command.c_str(), "recherche", strlen("recherche")) == 0)
	{
		//NOT IMPLEMENTED YET
	}
	else if (command == "quitter")
	{
		return;
	}
	else
	{
		cout << "commande non trouvee, veuillez reessayer ou entrez 'help' ou '?' pour afficher la liste des commandes" << endl;
	}
	readCommand(graph, layerGraph);
}

int main()
{
	cout << "\t Plus court chemin avec contraintes pour graphe par couche\n" << endl;
	cout << "Vous devez avoir deux .afdC, le premier pour le graphe, le deuxième pour les bornes.\n" << endl;
	cout << "Entrez le nom du fichier contenant les transitions : " << endl;
	string transitionFilName;
	cin >> transitionFilName;
	bool fileFound = false;
	AFDGraph* graph = nullptr;
	while (!fileFound)
	{
		try
		{
			graph = buildGraphTransitionFile(transitionFilName);
			fileFound = true;
		}
		catch (...)
		{
			cout << "\nErreur le fichier n'a pas ete trouve." << endl;
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
			LayerGraph layerGraph = buildLayerGraph(*graph, limitsFileName);
			fileFound = true;
			displayCommands();
			readCommand(*graph, layerGraph);
		}
		catch (...)
		{
			cout << "\nErreur le fichier n'a pas ete trouve." << endl;
			cout << "Entrez le nom du fichier contenant les transitions : " << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> limitsFileName;
		}

	}
	_CrtDumpMemoryLeaks();
	return 0;
}

