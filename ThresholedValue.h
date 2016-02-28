///////////////////////////////////////////////////////////////////////
// File:        ThresholedValue.h
// Description: Threshold Value Calculator.
// Author:      S. M. Murtoza Habib
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////


#pragma once

ref class ThresholedValue
{
private:
	static int num_bins = 256; 
	int xsize;
	int ysize;
	float *counts;
    float sum_counts;
    float *p;
    float *omega;
    float *mu;
    float mu_t;

	System::Drawing::Bitmap^ ImageFile;

	void ImHist();

public:
	ThresholedValue(System::Drawing::Bitmap^);
	~ThresholedValue();
	float GetThresholed();
	void calculateValue();
};
