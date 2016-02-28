///////////////////////////////////////////////////////////////////////
// File:        LayoutAnalysis.h
// Description: Page Layout Analyzer.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#include<vector>
#include "TextBlock.h"


#pragma once

using namespace std;

ref class LayoutAnalysis
{
private:
	bool **imgArray;
	int R;
	int C;

	System::Drawing::Bitmap^ plImage;
	System::Collections::ArrayList^ orderLayout;

public:
	LayoutAnalysis(bool **b, int x, int y);
	LayoutAnalysis(int **b, int x, int y);
	void AnalysisLayoutAndGenerateImage();
	vector<TextBlock> AnalysisLayout();
	vector<int> getTextBoundary(int thresholdValue);
	vector<int> getProjection(int thVal, char projType);
	vector<int> getShortProjection(int st, int ed, char projType);
	vector<int> getMargine(vector<int> proj, int st, int ed);
	vector<vector<int>> gapSearch(vector<int> Vb, int lm, int rm);
	vector<vector<int>> blockSearch(vector<vector<int>> GapInfo, int lm, int rm);
	float getMean(vector<int> a);
	float getStdDev(vector<int> a);
	System::Drawing::Bitmap^ getLayoutImage();
};
