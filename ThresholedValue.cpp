///////////////////////////////////////////////////////////////////////
// File:        ThresholedValue.cpp
// Description: Threshold Value Calculator.
// Author:      S. M. Murtoza Habib
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ThresholedValue.h"


using namespace System;
using namespace System::Collections;


ThresholedValue::ThresholedValue(System::Drawing::Bitmap^ bm)
{
	ImageFile = bm;

	xsize = ImageFile->Width;
	ysize = ImageFile->Height;
}

ThresholedValue::~ThresholedValue(){
	delete [] counts;
	delete [] p;
	delete [] omega;
	delete [] mu;
}

void ThresholedValue::ImHist()
{	
	counts = new float[256];

	for (int i = 0; i < 256; i++ ){
		counts[i] = 0;
	}

	int max = 0;
	for( int i = 0; i < xsize; i++ )
		for ( int j = 0; j < ysize; j++ ){
			System::Drawing::Color^ clr = ImageFile->GetPixel(i,j);
			int pixel = clr->ToArgb();				
			
			counts[(pixel >> 16) & 0xff]++; //red			
			counts[(pixel >>  8) & 0xff]++;	//green		
			counts[(pixel      ) & 0xff]++; //blue
		}// inner for*/
}// end of function ImHist()




void ThresholedValue::calculateValue()
{
	int i;        
	sum_counts = 0;
	
	
	for( i = 0; i < 256; i++ ){
		sum_counts += counts[i];
	}        

	p = new float[256];
	omega = new float[256];
	mu = new float[256];

	p[0] = counts[0]/sum_counts;
	omega[0] = p[0];        
	mu[0] = p[0]*1;

	for( i = 1; i < 256; i++ ){
		p[i] = counts[i]/sum_counts;
		omega[i] = omega[i-1] + p[i]; 
		mu[i] = mu[i-1] + p[i]*(i+1); 
	}
	mu_t = mu[255];
}//end of function calculateValue()


float ThresholedValue::GetThresholed()
{
	ImHist();

	calculateValue();

	float level = 0;
	double *sigma_b_squared;
	sigma_b_squared = new double[256];
	double maxval = 0;

	for (int i = 0; i < 255; i++ ){
		float div = omega[i] * (1 - omega[i]);
		if (div == 0)
			sigma_b_squared[i] = -1;
		else
		{
			float tmp = (mu_t * omega[i] - mu[i]);
			sigma_b_squared[i] = (tmp*tmp)/div;
		}

		if (maxval < sigma_b_squared[i] )
			maxval = sigma_b_squared[i];
	}// end of for */

	int count = 0;
	int sumI = 0;

	if (maxval > -1 ){
		for (int i = 0; i < 256; i++){
			if(maxval == sigma_b_squared[i] ){
				sumI += i+1;
				count++;
			}
		}//end of for
		float idx = sumI/count;
		level = (idx - 1)/(num_bins-1);
	}// end of if
	else{
		level = 0;
	}

	// free memory
	delete[] sigma_b_squared;

	return level;
}
