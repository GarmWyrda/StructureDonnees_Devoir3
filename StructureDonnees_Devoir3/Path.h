#pragma once

#include "State.h"
#include <vector>
using namespace std;

class Path
{
private:
	int cost;
	vector<int> lettersCount;
	vector<State*> path;
public:
	
	Path(int size);
	Path(const Path& path);
	virtual ~Path();
	int getCost() const;
	vector<State*>& getPath();
	vector<State> toVector();
	vector<int>& getLettersCount();
	void addState(State* state);
	void addFrontState(State* state);

	/*void Path::operator=(const Path &p);
	bool Path::operator==(const Path &p) const;
	bool Path::operator<=(const Path &p) const;
	bool Path::operator>=(const Path &p) const;
	bool Path::operator<(const Path &p) const;
	bool Path::operator>(const Path &p) const;
	friend ostream& operator<<(ostream& ostr, Path& path);*/

};