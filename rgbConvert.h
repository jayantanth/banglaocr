///////////////////////////////////////////////////////////////////////
// File:        rgbConvert.h
// Description: RGB to grayscale converter
// Author:      S. M. Murtoza Habib and Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#pragma once
using namespace System::Drawing;

ref class rgbConvert
{
private: 
	System::Drawing::Bitmap^ grayImage;
	System::Drawing::Bitmap^ bainaryImage;
	System::Drawing::Bitmap^ rgbImage;
	int xsize;
	int ysize;
	bool **BArray;
	int **GrayArray;

	static float coef01 = (float)0.2989;
	static float coef02 = (float)0.5870;
	static float coef03 = (float)0.1140;

public:

	rgbConvert(System::Drawing::Bitmap^ im)
	{
		rgbImage = im;
		xsize = rgbImage->Height;
		ysize = rgbImage->Width;
	}

	rgbConvert(int height, int width)
	{
		xsize = height;
		ysize = width;
	}

	int** getGrayImageArray ()
	{
		GrayArray = new int * [xsize];
		for (int i = 0; i < xsize; i++ )
		{
			GrayArray[i] = new int[ysize];
		}

		for( int i = 0; i < this->xsize; i++ )
		{
			for ( int j = 0; j < this->ysize; j++ )
			{
				System::Drawing::Color^ clr = this->rgbImage->GetPixel(j, i);
				int pixel = clr->ToArgb();				

				//int alpha = (pixel >> 24) & 0xff;// no need here
				int red = (pixel >> 16) & 0xff;
				int green = (pixel >>  8) & 0xff;
				int blue = (pixel      ) & 0xff;

				int grayC = int(coef01*red + coef02*green + coef03*blue);

				GrayArray[i][j] = grayC;
			}// inner for*/
		}

		return GrayArray;
	}

	void getGrayImageArray (int** gArray)
	{
		this->GrayArray = gArray;
	}

	bool** GetBinaryArray( int level )
	{
		BArray = new bool * [xsize];
		for (int i = 0; i < xsize; i++ )
		{
			BArray[i] = new bool[ysize];
		}

		int grayC;

		for ( int xVal = 0; xVal < xsize; xVal++ )
		{
			for( int yVal = 0; yVal < ysize; yVal++ )
			{
				grayC = GrayArray[xVal][yVal];

				if ( grayC >= level )
				{
					BArray[xVal][yVal] = true;
				}
				else
				{
					BArray[xVal][yVal] = false;
				}

			}// inner for*/
		}
		return BArray;
	}
};
