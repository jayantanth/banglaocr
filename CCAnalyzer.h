///////////////////////////////////////////////////////////////////////
// File:        CCAnalyzer.h
// Description: Connected component analyzer.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#include<vector>
#include<map>
#include "point.h"
#include "ConnCompProperties.h"

#pragma once

using namespace std;

ref class CCAnalyzer
{
private:
	int **LabelArray;
	bool **BArray;

	System::Collections::ArrayList^ Parent;
	System::Collections::ArrayList^ Rank;
	int xsize;
	int ysize;		
	int numberOflabels;
	bool advanceMood;
	System::Collections::ArrayList^ CCList;
	
public:
	CCAnalyzer(bool **b, int x, int y, bool advMood);
	CCAnalyzer(int **b, int x, int y, bool advMood);
	void labelCC();
	vector<vector<point>> labelCC_adv_mood();
	void makeSet(int num);
	void Union(int x, int y);
	int Find(int x);
	int getMinVal(vector<int> a);
	int renumber_labels(int start);
	vector<vector<point>> renumber_labels_adv_mood(int start);
	int getNumberOfCC();
	int** getLabeledArray();
	vector<point> getCCPoints(int label);
};
