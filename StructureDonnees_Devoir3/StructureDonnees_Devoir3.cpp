// StructureDonnees_Devoir3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
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

bool readTransitionFile(string fileName)
{
	ifstream infile(fileName);
	int alphabetSize;
	int nbStates;
	string initialState;
	int nbFinalStates;
	vector<string> finalStates = vector<string>();
	int nbTransition;
	string line;

	vector<vector<string>> transition = vector<vector<string>>();

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
		transition.push_back(split(line, ' '));
		numTransition++;
	}
	return true;
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

	while (!fileFound)
	{
		try
		{
			readTransitionFile(transitionFilName);
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


		system("pause");
	return 0;
}

