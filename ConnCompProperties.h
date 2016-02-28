///////////////////////////////////////////////////////////////////////
// File:        ConnCompProperties.h
// Description: Connected component properties container.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#include "point.h"
#include <vector>

#pragma once

using namespace std;

class ConnCompProperties
{
private:
	vector<vector<point>> Components;

public:
	ConnCompProperties(vector<vector<point>> cc);
	vector<int> getRegion(int compNumber);
	vector<vector<int>> getAllRegion();
	int getMinVal(vector<int> a);
	int getMaxVal(vector<int> a);
};
