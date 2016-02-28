///////////////////////////////////////////////////////////////////////
// File:        SkewDetectionCorrection.cpp
// Description: Skew angle detector and corrector of Bangla OCR.
// Author:      Md. Abul Hasnat and Murtoza Habib
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
// Acknowledgement: OCROpus group for the bilinear interpolation code.
///////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SkewDetectionCorrection.h"

using System::Math;

SkewDetectionCorrection::SkewDetectionCorrection(bool** b, int x, int y)
{
	this->findAnglaDone = false;

	this->fast = true;

	this->BArray = b;
	this->xsize = x;
	this->ysize = y;

	this->cca = gcnew CCAnalyzer(BArray, xsize, ysize, true);

	if (this->ysize > this->xsize)
		this->SIZE = this->ysize;
	else
		this->SIZE = this->xsize;

	this->UpperEnvelope = new int * [this->SIZE];
	for (int i = 0; i < this->SIZE; i++)
	{
		this->UpperEnvelope[i] = new int [this->SIZE];
		for (int j = 0; j < this->SIZE; j++)
		{
			this->UpperEnvelope[i][j] = 0;
		}
	}
}

void SkewDetectionCorrection::calMean (vector<int> CW)
		 {
			 int numOfComponent = CW.size();
			 this->Mean = 0;
			 for (int i = 0; i < numOfComponent; i++)
			 {
				 this->Mean += CW[i];
			 }
			 this->Mean /= numOfComponent;	
		 }

void SkewDetectionCorrection::calStandardDeviation(vector<int> CW)
		 {
			 int numOfComponent = CW.size();

			 this->StandardDeviation = 0;
			 for (int i = 0; i < numOfComponent; i++)
			 {
				 this->StandardDeviation += Math::Pow(CW[i] - this->Mean, 2.0);
			 }
			 this->StandardDeviation /= numOfComponent;

			 this->StandardDeviation = Math::Sqrt( this->StandardDeviation );
		 }

void SkewDetectionCorrection::findUpperEnvelope()
{
	// Get the connected components
	vector<vector<point>> ccS = cca->labelCC_adv_mood();

	// Get the properties of the connected components
	ConnCompProperties ccp(ccS);
	vector<vector<int>> regionsAll = ccp.getAllRegion();

	// Compute the width of the components
	vector<int> CCWidth;
	for(int rw=0; rw<regionsAll.size(); rw++){
		CCWidth.push_back(regionsAll[rw][3] - regionsAll[rw][1] + 1);
	}

	this->calMean (CCWidth);
	this->calStandardDeviation(CCWidth);

	int ** LabelArray = cca->getLabeledArray();

	for (int i = 0; i < CCWidth.size(); i++)
	{
		if (CCWidth[i] < this->Mean)
			continue;
		if (CCWidth[i] > this->Mean + 4 * this->StandardDeviation)
			continue;

		for (int c = regionsAll[i][1]; c <= regionsAll[i][3]; c++){
			for ( int r = regionsAll[i][0]; r <= regionsAll[i][2]; r++)
				if (LabelArray[r][c] == i+1)
				{
					this->UpperEnvelope[r][c] = 1;
					break;
				}
		}
	}
}// end findUpperEnvelope()

float SkewDetectionCorrection::GetSkewAngle()
	{    
		if (this->findAnglaDone)
		{
			return this->skewAngle;
		}

		////// testing ////////////
		int angleRange = 5;
		int scans = 256;
		float ang1 = 90 - angleRange;
		float ang2 = 90 + angleRange;
		float stepsize = (float)0.1;
		float vis = (ang2 - ang1)/stepsize;
		int views = (int)Math::Ceiling(vis) + 1;
		float *allAngle = new float[views];
		int ind = 0;
		for (float phi = ang1; phi < ang2; phi = phi+stepsize)
		{
			allAngle[ind] = phi;
			ind++;
		}
		////// end /////////////////


        double **proj;// = new double[views][scans];
		proj = new double *[views];
		for (int foo = 0; foo < views; foo++)
		{
			proj[foo] = new double [scans];
			for (int bar = 0; bar < scans; bar++ )
				proj[foo][bar] = 0;
		}
        double pos, val, Aleft, Aright;
        int x, y, Xcenter, Ycenter, Ileft, Iright;
        double *sintab = new double[views];
        double *costab = new double[views];		
		//S=0;
		int inputimgsize = this->SIZE;//this->xsize;// or this->ysize

		int i = 0;
		for (float phi = ang1; phi < ang2; phi = phi+stepsize)
		{
			sintab[i] = Math::Sin((double) phi * Math::PI / 180 - Math::PI/2);
			costab[i] = Math::Cos((double) phi * Math::PI / 180 - Math::PI/2);
            i++;
        }

		// Project each pixel in the image
        Xcenter = inputimgsize / 2;
        Ycenter = inputimgsize / 2;

        i=0;
        //if no. scans is greater than the image width, then scale will be <1

		double scale = inputimgsize*1.42/scans;

		int N=0; val = 0;
        double weight = 0;
		double sang = Math::Sqrt(2)/2;
        bool interrupt = false;
        double progr=0;

		int S;
		for (float phi = ang1; phi < ang2; phi = phi+stepsize)
		{
			if (interrupt) break;
			
			double a = -costab[i]/sintab[i] ;
			double aa = 1/a;

			if (Math::Abs(sintab[i]) > sang)
			{
				for (S = 0; S < scans; S++)
				{
					N = S - scans/2; //System.out.print("N="+N+" ");
					double b = (N - costab[i] - sintab[i]) / sintab[i];
					b =  b * scale;

					for (x = -Xcenter; x < Xcenter; x++)
					{
						if (fast == true)
						{
							//just use nearest neighbour interpolation
							y = (int) Math::Round(a*x + b);

							if (y >= -Xcenter && y < Xcenter )
								val += this->UpperEnvelope[(x+Xcenter)][(y+Ycenter)];

						}
						else 
						{
							//linear interpolation
							y = (int) Math::Round(a*x + b);
							weight = Math::Abs((a*x + b) - Math::Ceiling(a*x + b));

							if (y >= -Xcenter && y+1 < Xcenter )
								val += (1-weight) * this->UpperEnvelope[(x+Xcenter)][(y+Ycenter)]
							+ weight * this->UpperEnvelope[(x+Xcenter)][(y+Ycenter+1)];
						}
					} 
					proj[i][S] = val/Math::Abs(sintab[i]); 
					val=0;                    
				}
			}
			if (Math::Abs(sintab[i]) <= sang)
			{
				for (S = 0; S < scans; S++)
				{
					N = S - scans/2;
					double bb = (N - costab[i] - sintab[i]) / costab[i];
					bb = bb * scale;
					//IJ.write("bb="+bb+" ");
					for (y = -Ycenter; y < Ycenter; y++) 
					{
						if (fast ==true)
						{
							x = (int) Math::Round(aa*y + bb);
							if (x >= -Xcenter && x < Xcenter )
								val += this->UpperEnvelope[x+Xcenter][y+Ycenter];
						}
						else
						{
							x = (int) Math::Round(aa*y + bb);
							weight = Math::Abs((aa*y + bb) - Math::Ceiling(aa*y + bb));

							if (x >= -Xcenter && x+1 < Xcenter )
								val += (1-weight) * this->UpperEnvelope[(x+Xcenter)][(y+Ycenter)]
							+ weight * this->UpperEnvelope[(x+Xcenter+1)][(y+Ycenter)];
						}
					}
					proj[i][S] = val/Math::Abs(costab[i]); 
					val=0;
				}
			} 
			i++;			
		}
		
		double max = 0;
		for (int foo = 0; foo < views; foo++)
		{
			for (int bar = 0; bar < scans; bar++ )
			{
				if ( max < proj[foo][bar])
					max = proj[foo][bar];
			}		
		}

		int r, c;
		double Vmax = 0;
		for (int foo = 0; foo < views; foo++)
		{
			for (int bar = 0; bar < scans; bar++ )
			{				
				proj[foo][bar] = proj[foo][bar] * 255/max;
				if (Vmax < proj[foo][bar])
				{
					Vmax = proj[foo][bar];
					r = foo;
					c = bar;
				}
			}		
		}

		float tmp = 90 - allAngle[r];
		this->skewAngle = 90 - ( r*stepsize + ang1);
		this->findAnglaDone = true;
		return this->skewAngle;
	}

Bitmap^ SkewDetectionCorrection::RotateImg_DirectSample()
	{
		if(skewAngle == 0.0) 
		{
			Bitmap^ rImg =  gcnew Bitmap(ysize, xsize, System::Drawing::Imaging::PixelFormat::Format24bppRgb);

			for(int i=0;i<xsize;i++)
			{
				for(int j=0;j<ysize;j++)
				{
					if (BArray[i][j])
					{
						rImg->SetPixel(j, i, Color::White);
					}
					else
					{
						rImg->SetPixel(j, i, Color::Black);
					}
				}
			}

			return rImg;
		}

		float cx = xsize/2.0;
        float cy = ysize/2.0;

		bool **out;
		out = new bool * [xsize];
		for (int i = 0; i < xsize; i++)
		{
			out[i] = new bool [ysize];
			for (int j = 0; j < ysize; j++)
			{
				out[i][j] = false;
			}
		}	
        
		skewAngle = 0 - skewAngle;
		double angleRAD = Math::PI*skewAngle/180.0;

        float c = Math::Cos(angleRAD);
		float s = Math::Sin(angleRAD);
        int w = ysize;
		int h = xsize;
        if(cx>1e30) cx = w/2.0;
        if(cy>1e30) cy = h/2.0;
		
        for(int i=0;i<h;i++) {
            for(int j=0;j<w;j++) {

				float x = c * (i-cx) - s * (j-cy) + cx;
                float y = s * (i-cx) + c * (j-cy) + cy;
				
				out[i][j] = xref((int)Math::Round(x+0.5),(int)Math::Round(y+0.5));
            }
        }


		Bitmap^ rImg =  gcnew Bitmap(ysize, xsize, System::Drawing::Imaging::PixelFormat::Format24bppRgb);

		for(int i=0;i<h;i++)
		{
            for(int j=0;j<w;j++)
            {
				if ( 0 ==  out[i][j])
				{
					rImg->SetPixel(j, i, Color::Black);
				}
				else
				{
					rImg->SetPixel(j, i, Color::White);
				}
			}
		}

		return rImg;
	}

Bitmap^ SkewDetectionCorrection::RotateImg_DirectSample_WithResizeImage()
	{
		//skewAngle = 0 - skewAngle;
		double angleRAD = Math::PI*skewAngle/180.0;
		
		float cx = xsize/2.0;
        float cy = ysize/2.0;

        float c = Math::Cos(angleRAD);
		float s = Math::Sin(angleRAD);

        int w = ysize;
		int h = xsize;

        if(cx>1e30) cx = w/2.0;
        if(cy>1e30) cy = h/2.0;

		this->rYsize  = (int)Math::Round(Math::Abs(h*s) + w*c);
		this->rXsize  = (int)Math::Round(Math::Abs(w*s) + h*c);

		bool **out;

		out = new bool * [rXsize];
		for (int i = 0; i < rXsize; i++)
		{
			out[i] = new bool [rYsize];
			for (int j = 0; j < rYsize; j++)
			{
				out[i][j] = true;
			}
		}

		float teX;
		float teY;

		if(skewAngle>0)  
        {
            teX = cy*s - cx*(1-c);
			teY = cx*s - cy*(1-c);
		}else{
			teX = -cy*s - cx*(1-c);
			teY = -cx*s - cy*(1-c);
		}

        for(int i=0;i<h;i++) {
            for(int j=0;j<w;j++) {
				int x = Math::Round(c * (i-cx) - s * (j-cy) + cx + teX);
                int y = Math::Round(s * (i-cx) + c * (j-cy) + cy + teY);

				if (x<0)
					x = 0;
				else if (x>=rXsize)
					x = rXsize - 1;
				if (y<0)
					y = 0;
				else if (y>=rYsize)
					y = rYsize-1;

				out[x][y] = BArray[i][j];
            }
        }

		Bitmap^ rImg =  gcnew Bitmap(rYsize, rXsize, System::Drawing::Imaging::PixelFormat::Format24bppRgb);

		for(int i=0;i<rXsize;i++)
		{
            for(int j=0;j<rYsize;j++)
            {
				if (out[i][j])
				{
					rImg->SetPixel(j, i, Color::White);
				}
				else
				{
					rImg->SetPixel(j, i, Color::Black);
				}
			}
		}

		return rImg;
	}

int** SkewDetectionCorrection::RotateImgDirectInterpolate(float skAngle)
	{
		float cx = xsize/2.0;
        float cy = ysize/2.0;

		rotateImageArr = new int * [xsize];
		for (int i = 0; i < xsize; i++)
		{
			rotateImageArr[i] = new int [ysize];
			for (int j = 0; j < ysize; j++)
			{
				rotateImageArr[i][j] = 0;
			}
		}	
        
		skAngle = 0 - skAngle;
		double angleRAD = Math::PI*skAngle/180.0;

        float c = Math::Cos(angleRAD);
		float s = Math::Sin(angleRAD);
        int w = ysize;
		int h = xsize;
        if(cx>1e30) cx = w/2.0;
        if(cy>1e30) cy = h/2.0;
		
		int grayC;
        for(int i=0;i<h;i++) {
            for(int j=0;j<w;j++) {

				float x = c * (i-cx) - s * (j-cy) + cx;
                float y = s * (i-cx) + c * (j-cy) + cy;
				
				grayC = bilin(x,y);
				if(grayC>255){
					grayC=255;
				}

				rotateImageArr[i][j] = grayC;
            }
        }

		return rotateImageArr;
	}

int** SkewDetectionCorrection::RotateImg_DirectSample(float skAngle)
	{
		float cx = xsize/2.0;
        float cy = ysize/2.0;

		rotateImageArr = new int * [xsize];
		for (int i = 0; i < xsize; i++)
		{
			rotateImageArr[i] = new int [ysize];
			for (int j = 0; j < ysize; j++)
			{
				rotateImageArr[i][j] = 0;
			}
		}	
        
		skAngle = 0 - skAngle;
		double angleRAD = Math::PI*skAngle/180.0;

        float c = Math::Cos(angleRAD);
		float s = Math::Sin(angleRAD);
        int w = ysize;
		int h = xsize;
        if(cx>1e30) cx = w/2.0;
        if(cy>1e30) cy = h/2.0;
		
        for(int i=0;i<h;i++) {
            for(int j=0;j<w;j++) {

				float x = c * (i-cx) - s * (j-cy) + cx;
                float y = s * (i-cx) + c * (j-cy) + cy;
				
				rotateImageArr[i][j] = xref((int)Math::Round(x+0.5),(int)Math::Round(y+0.5));
            }
        }

		return rotateImageArr;
	}

System::Drawing::Bitmap^ SkewDetectionCorrection::getUpperEnvelopeImg()
	{
		this->imgUpperEnvelope =  gcnew Bitmap(ysize, xsize, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
		for ( int xVal = 0; xVal < this->xsize; xVal++ )
		{
			for( int yVal = 0; yVal < this->ysize; yVal++ )
			{				
				if ( 0 ==  this->UpperEnvelope[xVal][yVal])
				{
					this->imgUpperEnvelope->SetPixel(yVal, xVal, Color::Black);
				}
				else
				{
					this->imgUpperEnvelope->SetPixel(yVal, xVal, Color::Red);
				}

			}// inner for*/
		}
		return this->imgUpperEnvelope;
	}

int SkewDetectionCorrection::xref(int x, int y) {
        if (x<0)
            x = 0;
        else if (x>=xsize)
            x = xsize - 1;
        if (y<0)
            y = 0;
        else if (y>=ysize)
            y = ysize-1;

        return grayImageArr[x][y];
    }

int SkewDetectionCorrection::bilin(float x,float y) {
        int i = (int)x;
        int j = (int)y;
        float l = x-i;
        float m = y-j;
        float s00 = xref(i,j);
        float s01 = xref(i,j+1);
        float s10 = xref(i+1,j);
        float s11 = xref(i+1,j+1);
        int val = System::Math::Round((1.0-l) * ((1.0-m) * s00 + m * s01) + l * ((1.0-m) * s10 + m * s11));

		return val;
    }

void SkewDetectionCorrection::setGrayImageArray(int** imgArray){
	this->grayImageArr = imgArray;
}

void SkewDetectionCorrection::setYsize(int ys){
	this->ysize = ys;
}