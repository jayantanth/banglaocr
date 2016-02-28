///////////////////////////////////////////////////////////////////////
// File:        ConnCompProperties.cpp
// Description: Connected component properties container.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ConnCompProperties.h"

using namespace std;

ConnCompProperties::ConnCompProperties(vector<vector<point>> cc)
{
	Components = cc;
}

vector<int> ConnCompProperties::getRegion(int compNumber)
{
	vector<int> regionOfComponent;

	int numOfElements = Components[compNumber].size();

	vector<int> xPoints(numOfElements);
	vector<int> yPoints(numOfElements);

	for(int i=0; i<numOfElements; i++){
		xPoints[i] = Components[compNumber][i].x;
		yPoints[i] = Components[compNumber][i].y;
	}

	// left - x
	regionOfComponent.push_back(getMinVal(xPoints));

	// top - y
	regionOfComponent.push_back(getMinVal(yPoints));

	// right - x
	regionOfComponent.push_back(getMaxVal(xPoints));

	// bottom - y
	regionOfComponent.push_back(getMaxVal(yPoints));

	return regionOfComponent;
}

vector<vector<int>> ConnCompProperties::getAllRegion()
{
	int numOfElements = Components.size();

	vector<vector<int>> AllRegionOfComponents(numOfElements);

	for(int i=0; i<numOfElements; i++){
		AllRegionOfComponents[i] = getRegion(i);;
	}

	return AllRegionOfComponents;
}

int ConnCompProperties::getMaxVal(vector<int> a){
	int limit = a.size();
	int maxVal = a[0];
	for(int i=1;i<limit;i++){
		if(a[i]>maxVal){
			maxVal=a[i];
		}
	}
	return maxVal;
}

int ConnCompProperties::getMinVal(vector<int> a){
	int limit = a.size();
	int minVal=a[0];
	for(int i=1;i<limit;i++){
		if(a[i]<minVal){
			minVal=a[i];
		}
	}
	return minVal;
}