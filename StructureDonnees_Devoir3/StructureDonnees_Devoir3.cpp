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
	vector<string> minValues, maxValues = vector<string>();
	vector<int> minValuesInt, maxValuesInt = vector<int>();
	string line;

	vector<vector<string>> transition = vector<vector<string>>();

	getline(infile, line);
	alphabetSize = stoi(line);
	getline(infile, line);
	wordSize = stoi(line);
	getline(infile, line);
	minValues = split(line, ' ');
	for (string minValue : minValues) minValuesInt.push_back(stoi(minValue));
	getline(infile, line);
	maxValues = split(line, ' ');
	for (string maxValue : maxValues) maxValuesInt.push_back(stoi(maxValue));

	return LayerGraph(graph, wordSize, minValuesInt, maxValuesInt);
}

void displayCommands()
{
	cout << "---------- Les commandes possibles sont les suivantes ----------\n" << endl;
	cout << "- help, ? : affiche la liste des commandes" << endl;
	cout << "- graphe : affiche à l'ecran les noeuds du graphe AFD" << endl;
	cout << "- lgraphe : affiche à l'ecran les noeuds du graphe par couche" << endl;
	cout << "- plus court chemin : calcule et affiche le plus court chemin" << endl;
	cout << "- plus court chemin sans contraintes : calcule et affiche le plus court chemin sans les contraintes du fichier limite" << endl;
	cout << "- recherche : recherche une arete." << endl;
	cout << "- quitter : quitte le programme.\n" << endl;

}

void printShortestPath(vector<State> path)
{
	if (!path.empty())
	{
		cout << "Les noeuds parcourus sont : ";
		for (size_t i = path.size() - 1; i > 0; i--)
		{
			if (path[i].getId() != -1 || path[i].getId() != -2)
			{
				cout << path[i].getId() << "-";
			}
		}
		
		if(path.front().getNodeState().getCost() != 0)
		{
			cout << "\nCout total : " << path.front().getNodeState().getCost() << endl;
		}
		
		cout << "Le mot construit est : ";
		for (size_t i = path.size() - 1; i > 0; i--)
		{
			if (path[i].getId() != -1 || path[i].getId() != -2)
			{
				int arrivalId = path[i - 1].getId();
				vector<Edge> previousStateTransitions = path[i].getTransitions();
				vector<Edge>::iterator itTransition = find_if(previousStateTransitions.begin(), previousStateTransitions.end(), [arrivalId](const Edge& transition) {return transition.getArrivalState()->getId() == arrivalId;});
				cout << itTransition->getTransition() << " ";
			}
		}
		cout << endl;
	}
	else
	{
		cout << "Erreur lors du calcul du plus court chemin" << endl;
	}
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
	else if (command == "plus court chemin")
	{
		vector<State> path = layerGraph.findShortestPathWithLimits();
		printShortestPath(path);
	}
	else if (command == "plus court chemin sans contraintes") {
		vector<State> path = layerGraph.findShortestPath();
		printShortestPath(path);
	}
	else if (command == "recherche")
	{
		string input;
		cout << "Entrez le premier noeud de l'arete recherchee." << endl;
		getline(cin, input);
		int idNode1 = stoi(input);

		cout << "Entrez son niveau dans le graphe en couche" << endl;
		getline(cin, input);
		int layer1 = stoi(input);

		State* state1 = layerGraph.getState(idNode1, layer1);
		cout << "Entrez le deuxieme noeud de l'arete recherchee." << endl;
		getline(cin, input);
		int idNode2 = stoi(input);

		cout << "Entrez son niveau dans le graphe en couche" << endl;
		getline(cin, input);
		int layer2 = stoi(input);

		State* state2 = layerGraph.getState(idNode2, layer2);
		Edge* edge = layerGraph.getEdge(state1, state2);

		if (edge == nullptr) {
			cout << "Il n'existe pas de transition directe entre ces deux noeuds" << endl;
		}
		else {
			cout << "Entrez le cout maximum souhaitee entre les deux noeuds (Laissez vide pour un nombre illimite)." << endl;
			getline(cin, input);
			if (input != "") {
				int cost;
				cost = stoi(input);
				vector<State> path = layerGraph.getEdgeOnValidPathWithCost(edge, cost);
				if (path.empty()) {
					cout << "Erreur: il n'existe aucun chemin entre ces deux noeuds avec un cout de " << cost << endl;
				}
				else {
					cout << "Un chemin a ete trouve." << endl;
					printShortestPath(path);
				}
			}
			else {
				vector<State> path = layerGraph.getEdgeOnValidPath(edge);
				if (path.empty()) {
					cout << "Erreur: il n'existe aucun chemin entre ces deux noeuds" << endl;
				}
				else {
					cout << "Un chemin a ete trouve." << endl;
					printShortestPath(path);
				}
			}
		}
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

