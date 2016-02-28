///////////////////////////////////////////////////////////////////////
// File:        SkewDetectionCorrection.h
// Description: Skew angle detector and corrector of Bangla OCR.
// Author:      Md. Abul Hasnat and Murtoza Habib
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
// Acknowledgement: OCROpus group for the bilinear interpolation code.
///////////////////////////////////////////////////////////////////////


#include "CCAnalyzer.h"
#include <vector>
#include "ConnCompProperties.h"

using namespace std;

using System::Drawing::Bitmap;
using System::Drawing::Color;

#pragma once

ref class SkewDetectionCorrection
{
private:
	System::Drawing::Bitmap^ imgUpperEnvelope;

	int** UpperEnvelope;
	int** rotateImageArr;
	int** grayImageArr;
	bool **BArray;
	
	bool fast;

	int xsize;
	int ysize;
	int rXsize;
	int rYsize;

	int SIZE;
	CCAnalyzer^ cca;
	double skewAngle;
	bool findAnglaDone;
	
	double Mean;
	double StandardDeviation;
	int outImageHeight;
	int outImageWidth;
	bool **outImageMap;

	void calMean (vector<int> CW);
	void calStandardDeviation(vector<int> CW);

public:
	SkewDetectionCorrection(bool** b, int x, int y);
	void findUpperEnvelope();
	float GetSkewAngle();
	System::Drawing::Bitmap^ RotateImg_DirectSample();
	System::Drawing::Bitmap^ RotateImg_DirectSample_WithResizeImage();
	int** RotateImgDirectInterpolate(float skAngle);
	int** RotateImg_DirectSample(float skAngle);
	System::Drawing::Bitmap^ getUpperEnvelopeImg();
	int xref(int x, int y);
	int bilin(float x,float y);
	void setGrayImageArray(int** imgArray);
	void setYsize(int ys);
};
