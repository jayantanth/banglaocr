///////////////////////////////////////////////////////////////////////
// File:        HSegmenter.h
// Description: Character Segmenter for Bangla OCR.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#pragma once
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "UnitInformation.h"
#include "CCAnalyzer.h"
#include "point.h"

using namespace std;

ref class HSegmenter
{
private:
	int **I;
	int xsize; 
	int ysize;
	System::IO::StreamWriter^ sw;
	int fileNum;

public:
	int numOfElementsPerUnit;
	CCAnalyzer^ cca;

	bool segmentSuccess;

	HSegmenter(bool **b, int x, int y);
	HSegmenter(bool **b, int x, int y, int num);
	~HSegmenter();

	vector<UnitInformation> SegmentWords();
	vector<vector<int> > eliminateMBJError(vector<vector<int> > PUnits, int** Word);
	vector<vector<int> > eliminateSplittingError(vector<vector<int> > PUnits);
	vector<vector<int>> getLines(int lineThresholdValue);
	vector<vector<int>> getWords(int** I, int r, int c);
	vector<vector<int>> getUnits(int** I, int numRow, int numCol, int start, int end);

	vector<int> rlhist(int** I, int r, int c);
	int** allocate2DIntArray(int r, int c);
	bool** allocate2DBoolArray(int r, int c);
	void delete2DIntArray(int** array, int r);
	void delete2DBoolArray(bool** array, int r);
	float getMean(int* a, int N);
	float getMean(vector<int> a);
	float getMedian(int* a, int N);
	float getMedian(vector<int> a);
	float getMedian2(vector<int> a);
	float getStdDev(int* a, int N);
	float getStdDev(vector<int> a);
	int getMaxVal(int* a, int limit);
	int getMaxVal(vector<int> a);
	int getMaxIndx(int* a, int limit);
	int getMaxIndx(vector<int> a);
	int getMaxIndxInRange(int* a, int start, int end);
	int getMaxIndxInRange(vector<int> a,int start, int end);
	int getMinVal(int* a, int limit);
	int getMinVal(vector<int> a);
	int getMinIndx(int* a, int limit);
	int getMinIndx(vector<int> a);
	int getMinIndxInRange(int* a, int start, int end);
	int getMinIndxInRange(vector<int> a, int start, int end);
	int getSum(char flag,int num,int** I, int r, int c);
	int getSumLR(int** I, int num, int left, int right);
	int** cropingImage(int** Simage, int top, int bottom, int left, int right);
	void getMatraaLocPH(int** TWord, int r1, int c1, int &ed_index_up_zone_ph, int &st_index_ml_zone_ph);
	vector<point> find(int color, int** I, int r, int c);
	vector<vector<int>> sortByRows(vector<vector<int>> arrayToSort, int numOfElement, int index);
	vector<UnitInformation> getJoiningErrorFreeUnits(int** I, vector<UnitInformation> LineWordsInfo);
	vector<UnitInformation> getSplittingErrorFreeUnitsOfALine(vector<UnitInformation> LineUnitsInfo);
	vector<vector<int> > getUnitsFromTouchingPoints(vector<int> TPoints, int** SI,  int numR, int numC);
	vector<int> copyDesiredColumn(vector<vector<int>> srcArray, int columnToCopy);
	bool checkValidityUsingBottom(int** I, int numR, int numC);
	vector<int> eliminateTouchingError(int** I, int numR, int numC);
	vector<int> getCrossing(int** I, int numR, int numC);
	int getVerticalBar(int** I, int numR, int numC);
};
