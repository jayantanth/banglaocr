///////////////////////////////////////////////////////////////////////
// File:        HSegmenter.cpp
// Description: Character Segmenter for Bangla OCR.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "HSegmenter.h"

using namespace std;


HSegmenter::HSegmenter(bool **b, int xSz, int ySz, int num){
	this->fileNum = num;
	this->xsize = xSz;
	this->ysize = ySz;

	this->I = allocate2DIntArray(xsize, ysize);
	
	for(int x=0; x<xSz; x++){
		for(int y=0; y<ySz; y++){
			if(b[x][y]){
				this->I[x][y] = 1;
			}else{
				this->I[x][y] = 0;
			}
		}
	}

	segmentSuccess = true;
}

HSegmenter::HSegmenter(bool **b, int xSz, int ySz){
	this->xsize = xSz;
	this->ysize = ySz;

	this->I = allocate2DIntArray(xsize, ysize);
	
	for(int x=0; x<xSz; x++){
		for(int y=0; y<ySz; y++){
			if(b[x][y]){
				this->I[x][y] = 1;
			}else{
				this->I[x][y] = 0;
			}
		}
	}

	segmentSuccess = true;
}

HSegmenter::~HSegmenter(){
	// free I
	for(int x=0; x<this->xsize; x++){
		delete I[x];
	}

	delete[] I;
}

vector<UnitInformation> HSegmenter:: SegmentWords()
{
	vector<UnitInformation> blockUnitsInformation;
	
	int lineThresholdValue = 6;
	vector<vector<int>> lineInfo = getLines(lineThresholdValue);
	int numOfLine = lineInfo.size();

	if(segmentSuccess){
		int thValLineHt = 5; 
		int lineHt;
		int** Line;
		int lineRSz;
		int lineCSz;	
		int wordStart;
		int wordEnd;
		vector<vector<int>> sortedUnitsInfo;
		int yy;
		int lstrt;

		for(int i=0; i<numOfLine; i++){
			lineHt = lineInfo[i][1] - lineInfo[i][0] + 1;
			if(lineHt <= thValLineHt){
				continue;
			}

			/*if(i<11){
				continue;
			}else{
				lineHt = lineInfo[i][1] - lineInfo[i][0] + 1;
			}*/

			/* * * *  Segment Words  * * * */
			Line = cropingImage(I, lineInfo[i][0], lineInfo[i][1], 0, ysize);

			// Get the word boundaries
			lineRSz = lineInfo[i][1]-lineInfo[i][0]+1;
			lineCSz = ysize;

			vector<vector<int>> WordInfo = getWords(Line, lineRSz, lineCSz);
			int numOfWord = WordInfo.size();
			
			///
			/*sw = gcnew System::IO::StreamWriter("c:\\"+fileNum+".txt", true);
			sw->WriteLine("Word Segmentation for line " +i+ " completed");	
			sw->Close();*/
			///

			vector<UnitInformation> LineWordsInfo;
			vector<vector<int>> UnitsInfo;

			for(int wc=0; wc<numOfWord; wc++){
				wordStart = WordInfo[wc][0];
				wordEnd = WordInfo[wc][1];

				UnitsInfo = getUnits(Line, lineRSz, lineCSz, wordStart, wordEnd);

				if(UnitsInfo.size()<1){
					continue;
				}

				// sort the units info by left boundary information
				sortedUnitsInfo = sortByRows(UnitsInfo, numOfElementsPerUnit, 2);

				// copy the information into LineWordsInfo
				yy = WordInfo[wc][0];

				int numOfUnit = UnitsInfo.size();

				for(int y=0; y<numOfUnit; y++){
					UnitInformation uinfo;				

					// set the actual height
					uinfo.height = sortedUnitsInfo[y][4];

					// set the actual width
					uinfo.width = sortedUnitsInfo[y][5];

					// set top info
					uinfo.top = sortedUnitsInfo[y][0];

					// set bottom info
					uinfo.bottom = sortedUnitsInfo[y][1];

					// set left info
					uinfo.left = yy + sortedUnitsInfo[y][2];

					// set right info
					uinfo.right = yy + sortedUnitsInfo[y][3];

					// set the matraa starting position
					uinfo.matraaSrartLoc = sortedUnitsInfo[y][6];

					// set the matraa ending positionrig
					uinfo.matraaEndLoc = sortedUnitsInfo[y][7];

					// set the line number
					uinfo.lineNumber = 1;

					// set the word number
					uinfo.wordNumber = wc;

					// set the unit number
					uinfo.unitNumber = y;

					// set the zone info
					uinfo.zoneNumber = sortedUnitsInfo[y][8];

					// set the shadow units location as -1
					uinfo.locOfShadowUnit = -1;

					LineWordsInfo.push_back(uinfo);
				}

				// free memory
				sortedUnitsInfo.~vector();
				UnitsInfo.~vector();
			}

			// free memory
			WordInfo.~vector();

			// Get Joining Error Free Units
			vector<UnitInformation> LineUnitsInfo;
			LineUnitsInfo = getJoiningErrorFreeUnits(Line, LineWordsInfo);
			LineWordsInfo.~vector();

			// Get Splitting Error Free Units
			vector<UnitInformation> AllUnitsInfo;
			AllUnitsInfo = getSplittingErrorFreeUnitsOfALine(LineUnitsInfo);
			LineUnitsInfo.~vector();

			lstrt = lineInfo[i][0];

			int sizeOfAllUnit = AllUnitsInfo.size();
			for(int x=0; x<sizeOfAllUnit; x++){
				AllUnitsInfo[x].top = lstrt + AllUnitsInfo[x].top;
				AllUnitsInfo[x].bottom = lstrt + AllUnitsInfo[x].bottom;
				// Add this unit information into the list
				blockUnitsInformation.push_back(AllUnitsInfo[x]);
			}

			//// free memory
			AllUnitsInfo.~vector();
			delete2DIntArray(Line, lineRSz);
		}

		//// free memory
		lineInfo.~vector();
	}

	return blockUnitsInformation;
}

vector<vector<int>> HSegmenter::getLines(int lineThreshold){
	vector<vector<int>> lineInfo;

	int r = this->xsize;
	int c = this->ysize;

	vector<int> RLHist = rlhist(this->I, r, c);

	int thValLineHt = 10; 
	int thValMinima = lineThreshold; 
	int count = -1;
	vector<int> Minima;

	if(RLHist[0] != 0){
		count++;
		Minima.push_back(0);
	}

	for (int i = 1 ; i<r; i++){
		if(RLHist[i] <= thValMinima){
			count++;
			Minima.push_back(i);
		}
	}

	if(r-Minima[count] > thValLineHt){
		count++;
		Minima.push_back(r-1);
	}

	vector<int> FRS_;
	vector<int> LRS_;
	vector<int> HS_;
	vector<int> FRS;
	vector<int> LRS;
	vector<int> HS;
	vector<int> RLH;
	vector<int> RLH2;
	vector<int> SPHline;
	vector<int> EPHline;

	int count_ = -1;

	
	for (int i = 1; i<=count; i++){
		if(Minima[i] - Minima[i-1] > 1){
			count_++;
			FRS_.push_back(Minima[i-1]);
			LRS_.push_back(Minima[i]);
		}
	}
	
	count = -1;

	for (int i=0; i<=count_; i++){
		count++;
		HS_.push_back(LRS_[count] - FRS_[count] + 1);
	}

	int numberOfLine = count;

	// Statistical information about the lines
	float avgHt = getMean(HS_);
	float medianHt = getMedian2(HS_);
	float stdHt = getStdDev(HS_);

	/// Module to prevent over segmentation
	int vCount = -1;
	vector<int> Validity;

	for (int i= 0; i<=numberOfLine; i++){
		vCount++;
		if(HS_[i] < abs(avgHt - stdHt) ){
			Validity.push_back(0);
		}else{
			Validity.push_back(1);
		}
	}

	vCount = -1;
	for (int i= 0; i<=numberOfLine; i++){
		vCount++;
		if(Validity[i]==0){
			if(i<numberOfLine && (LRS_[i]-FRS_[i+1])==0){
				FRS.push_back(FRS_[i]);
				LRS.push_back(LRS_[i+1]);
				HS.push_back(HS_[i]+HS_[i+1]);
				i++;
			}
			else if(i>0 && i<numberOfLine && (FRS_[i]-LRS_[i+1])==0){
				FRS.pop_back();	
				FRS.push_back(FRS_[i-1]);
				LRS.pop_back();
				LRS.push_back(LRS_[i]);
				HS.pop_back();
				HS.push_back(HS_[i-1]+HS_[i]);
			}
			else{
				FRS.push_back(FRS_[i]);
				LRS.push_back(LRS_[i]);
				HS.push_back(HS_[i]);
			}
		}else{
			FRS.push_back(FRS_[i]);
			LRS.push_back(LRS_[i]);
			HS.push_back(HS_[i]);
		}
	}

	numberOfLine = HS.size()-1;
	/// End of Module to prevent over segmentation
	// Average gap between lines
	vector<int> Gap;
	for (int i = 0; i<=numberOfLine-1; i++){
		Gap.push_back(FRS[i+1] - LRS[i]);
	}

	float averageGap;
	float medianGap;

	if(Gap.size()<1){
		averageGap = 0;
		medianGap = 0;
	}else{
		averageGap = getMean(Gap);
		medianGap = getMedian2(Gap);		
	}

	/////
	// identify the position of headlines using horizontal projections.
	/////
	bool matraaCorrect = true;
	int firstRowOfMatraa = 0;
	int lastRowOfMatraa = 0;
	int stIndx = 0;
	int endIndx = 0;
	int inDx = 0;
	int maxVal = 0;
	int minVal = 0;
	int heightOfCurrentLine = 0;
	int currentUpperZoneHt = 0;

	for(int i = 0; i<=numberOfLine; i++){
		if(HS[i] > thValLineHt){
			// find the peak index trhough run length histogram value
			// RLH = RLHist(FRS(i) : LRS(i));
			RLH.clear();
			for(int rlhIndex = FRS[i]; rlhIndex <= LRS[i]; rlhIndex++)
			{
				RLH.push_back(RLHist[rlhIndex]);	
			}

			inDx = getMaxIndx(RLH);
			maxVal = getMaxVal(RLH);

			// find the first row of the matraa / headline
			firstRowOfMatraa = inDx;

			stIndx = inDx-1;
			endIndx = 1;

			for(int j = stIndx; j>=endIndx; j--){
				if(RLH[j] >= maxVal * 0.7){
					firstRowOfMatraa = j;
				}else{
					break;
				}
			}

			SPHline.push_back(FRS[i] + firstRowOfMatraa);

			// check whether the matraa is located properly or not?
			heightOfCurrentLine = HS[i];

			if(i>1 && (FRS[i]-LRS[i-1] < averageGap/2)){
				currentUpperZoneHt = firstRowOfMatraa - averageGap;
			}else{
				currentUpperZoneHt = firstRowOfMatraa;
			}

			while (currentUpperZoneHt > ceil(0.4 * heightOfCurrentLine)){
				heightOfCurrentLine = SPHline[i]- FRS[i] + 1;

				// Incorrect headline detected
				//RLH = RLHist(FRS(i) : SPHline(i)-1);
				RLH2.clear();
				for(int rlhIndex = FRS[i]; rlhIndex <= SPHline[i]-1; rlhIndex++)
				{
					RLH2.push_back(RLHist[rlhIndex]);	
				}

				inDx = getMaxIndx(RLH2);
				maxVal = getMaxVal(RLH2);

				firstRowOfMatraa = inDx;

				stIndx = inDx-1;
				endIndx = 1;

				for(int j = stIndx; j>=endIndx; j--){
					if(RLH2[j] >= maxVal * 0.7){
						firstRowOfMatraa = j;
					}else{
						break;
					}
				}

				SPHline[i] = FRS[i] + firstRowOfMatraa - 1;

				currentUpperZoneHt = firstRowOfMatraa;
			} // end of while loop

			// find the last row of the matraa / headline
			lastRowOfMatraa = inDx;

			stIndx = inDx+1;
			endIndx = inDx + (HS[i] - inDx)/2;

			for(int j = stIndx; j<=endIndx; j++){
				if(RLH[j] >= maxVal * 0.7){
					lastRowOfMatraa = j;
				}else{
					break;
				}
			}

			EPHline.push_back(FRS[i] + lastRowOfMatraa);
		}else{ // if(HS(i) > averageHS * 0.3)
			SPHline.push_back(FRS[i]);
			EPHline.push_back(LRS[i]);
		} // if(HS(i) > averageHS * 0.3)
	}

	/////
	// Calculate average upper & middle zone height, length of matraa
	/////    
	vector<int> UzHt;
	vector<int> MLzHt;
	vector<int> MatraaLength;
	vector<int> USLSingle;
	vector<vector<int>> USL;

	for (int i = 0; i<=numberOfLine; i++){
		UzHt.push_back(SPHline[i] - FRS[i]);
		MLzHt.push_back(LRS[i] - SPHline[i]);
		MatraaLength.push_back(EPHline[i] - SPHline[i] + 1);
	}

	float avgMLzHt = getMean(MLzHt);
	float medianMLzHt = getMedian2(MLzHt);
	float stdMLzHt = getStdDev(MLzHt);
	float avgUzHt = getMean(UzHt);

	/////
	// Find out the correct and incorrect lines
	/////
	float upperThreshold = medianMLzHt * 1.5;
	//float upperThreshold = medianMLzHt * 1.25;
	float lowerThreshold = medianMLzHt * 0.5;

	bool extractDone;
	int searchStartIndex = 0;
	int searchEndIndex = 0;
	int currentLineHt = 0;
	int inDxMinVal = 0;
	int rlhSz = 0;

	for(int i = 0; i<=numberOfLine; i++){
		// Line extraction of under segmented lines
		if(MLzHt[i] > upperThreshold){
			USLSingle.push_back(i); // set the index of the under segmented line
			extractDone = true;
			while(extractDone){
				extractDone = false;
				// find the peak index trhough run length histogram value

				// Set the search start and end point
				searchStartIndex = EPHline[i] + (avgUzHt*0.5);
				if(EPHline[i]+upperThreshold+averageGap < LRS[i]){
					searchEndIndex = EPHline[i]+upperThreshold + averageGap;
				}else{
					searchEndIndex = LRS[i];
				}

				if(searchEndIndex<=searchStartIndex){
					USLSingle.push_back(FRS[i]);
					USLSingle.push_back(LRS[i]);
					extractDone = false;
					break;
				}

				// Search space           
				//RLH = RLHist(EPHline(i) : searchEndIndex);
				RLH.clear();
				for(int rlhIndex = searchStartIndex; rlhIndex <= searchEndIndex; rlhIndex++)
				{
					RLH.push_back(RLHist[rlhIndex]);	
				}

				inDx = getMaxIndx(RLH);
				maxVal = getMaxVal(RLH);

				currentLineHt = inDx - 1 - averageGap;

				if((currentLineHt < upperThreshold) && (currentLineHt > lowerThreshold)){
					// find the first row of the matraa / headline
					firstRowOfMatraa = inDx;

					stIndx = inDx-1;
					endIndx = 1;

					for(int j = stIndx; j>=endIndx; j--){
						if(RLH[j] >= maxVal * 0.7){
							firstRowOfMatraa = j;
						}else{
							break;
						}
					}

					SPHline[i] = searchStartIndex + firstRowOfMatraa;

					// correct line
					// [minVal inDxMinVal] = min(RLH(1 : firstRowOfMatraa-1));
					vector<int> LineBoundary;
					int endrlhIndex = firstRowOfMatraa-(avgUzHt*0.5);
					for(int rlhIndex = 0; rlhIndex <= endrlhIndex; rlhIndex++)
					{
						LineBoundary.push_back(RLH[rlhIndex]);	
					}
					inDxMinVal = getMinIndx(LineBoundary);
					minVal = getMinVal(LineBoundary);
					LineBoundary.~vector();


					USLSingle.push_back(FRS[i]);
					USLSingle.push_back(searchStartIndex + inDxMinVal);

					FRS[i] = searchStartIndex + inDxMinVal + 1;

					// find the last row of the matraa / headline
					lastRowOfMatraa = inDx;

					stIndx = inDx+1;
					endIndx = inDx + (HS[i] - inDx)/2;

					rlhSz = RLH.size() - 1;

					if(stIndx > rlhSz){
						lastRowOfMatraa = rlhSz;
					}else{
						if(endIndx > rlhSz){
							endIndx = rlhSz;
						}

						for(int j = stIndx; j<=endIndx; j++){
							if(RLH[j] >= maxVal * 0.7){
								lastRowOfMatraa = j;
							}else{
								break;
							}
						}
					}

					HS[i] = LRS[i] - FRS[i];

					EPHline[i] = EPHline[i] + lastRowOfMatraa + 1;

					extractDone = true;
				}else{

					USLSingle.push_back(FRS[i]);
					USLSingle.push_back(LRS[i]);
					extractDone = false;
				}
			} // end of while

			USL.push_back(USLSingle);
			USLSingle.~vector();
		}
	}

	/////////////////////////////////////////
	// CREATE LINKAGE
	/////////////////////////////////////////

	int numberOfUnderSegmentedLine = USL.size();

	if(numberOfUnderSegmentedLine>0){
		vector<int> tmpLineInfo;
		vector<int> tmpLineInfo2;
		int undersegmentedLineNumber = 0;
		int numLine = 0;
		int lineCountStart = 0;
		int lineCountEnd = 0;

		for(int i=0; i<numberOfUnderSegmentedLine; i++){
			tmpLineInfo.clear();
			tmpLineInfo = USL[i];
			undersegmentedLineNumber = tmpLineInfo[0];

			// copy the previous lines
			lineCountEnd = undersegmentedLineNumber-1;
			
			if(lineCountStart != undersegmentedLineNumber){
				for(int lci=lineCountStart; lci<=lineCountEnd; lci++){
					tmpLineInfo2.clear();
					tmpLineInfo2.push_back(FRS[lci]);
					tmpLineInfo2.push_back(LRS[lci]);
					lineInfo.push_back(tmpLineInfo2);
				}
			}

			// copy the under segmented lines
			numLine = (tmpLineInfo.size() - 1)/2;
			for(int li=0; li<numLine; li++){
				tmpLineInfo2.clear();
				tmpLineInfo2.push_back(tmpLineInfo[(int)li*2+1]);
				tmpLineInfo2.push_back(tmpLineInfo[(int)li*2+2]);
				lineInfo.push_back(tmpLineInfo2);
			}

			// set the next start index
			lineCountStart = undersegmentedLineNumber+1;
		}

		if(undersegmentedLineNumber < numberOfLine){
			for(int lci=lineCountStart; lci<=numberOfLine; lci++){
				tmpLineInfo2.clear();
				tmpLineInfo2.push_back(FRS[lci]);
				tmpLineInfo2.push_back(LRS[lci]);
				lineInfo.push_back(tmpLineInfo2);
			}
		}

		tmpLineInfo.~vector();
		tmpLineInfo2.~vector();
	}else{
		vector<int> tmpLineInfo;
		for(int li=0; li<=numberOfLine; li++){
			tmpLineInfo.clear();
			tmpLineInfo.push_back(FRS[li]);
			tmpLineInfo.push_back(LRS[li]);
			lineInfo.push_back(tmpLineInfo);
		}
		tmpLineInfo.~vector();
	}
	// free memory
	FRS_.~vector();
	LRS_.~vector();
	HS_.~vector();
	FRS.~vector();
	LRS.~vector();
	HS.~vector();
	RLH.~vector();
	SPHline.~vector();	
	EPHline.~vector();
	Gap.~vector();

	UzHt.~vector();
	MLzHt.~vector();
	MatraaLength.~vector();
	USL.~vector();
	USLSingle.~vector();

	RLHist.~vector();
	Minima.~vector();
	
	return lineInfo;
}

vector<vector<int>> HSegmenter::getWords(int** I, int r, int c){
	vector<vector<int>> WordInfo;
	vector<int> Vhist;
	for(int i=0;i<c;i++){
		Vhist.push_back(r - getSum('c',i, I, r, c));
	}

	int count = -1;
	int startIndex = 0;

	vector<int> Minima;
	if(Vhist[0] != 0){
		count = count + 1;
		Minima.push_back(-1);
		startIndex = 1;
	}

	int thVal = 1;
	for(int i=startIndex; i<c; i++){
		if(Vhist[i] < thVal){
			count = count + 1;
			Minima.push_back(i);
		}
	}

	if(Vhist[c-1] != 0){
		count = count + 1;
		Minima.push_back(c);
	}

	thVal = 1;
	int count_ = -1;
	vector<vector<int>> Gaps;
	vector<int> tempGap;

	for (int i=1;i<=count;i++){
		if(Minima[i] - Minima[i-1] > thVal){
			count_ = count_ + 1;
			tempGap.push_back(Minima[i-1] + 1);
			tempGap.push_back(Minima[i] - 1);
			Gaps.push_back(tempGap);
			tempGap.clear();
		}
	}
	tempGap.~vector();


	vector<int> Gapw;

	for(count = 0;count<count_;count++){
		Gapw.push_back(Gaps[count+1][0] - Gaps[count][1]);
	}

	if(count_ > 0){
		int size = count_;
		float avgGap=getMean(Gapw);
		int medianGap = getMedian(Gapw);
		float stdGap = getStdDev(Gapw);

		int largeGapPresent;	
		if ( stdGap - avgGap   > medianGap){
			largeGapPresent = 1;
		}else{
			largeGapPresent = 0;
		}

		while(largeGapPresent == 1){
			int maxValLoc = getMaxIndx(Gapw);
			Gapw[maxValLoc] = medianGap;

			avgGap = getMean(Gapw);
			medianGap = getMedian(Gapw);
			stdGap = getStdDev(Gapw);

			if ( avgGap - stdGap  > medianGap){
				largeGapPresent = 1;
			}else{
				largeGapPresent = 0;
			}
		}

		float gapFactor=0;
		if(medianGap < stdGap){
			gapFactor = avgGap;
		}
		else{
			gapFactor = stdGap;
		}

		vector<int> GapLoc;

		int wordCount = 1;
		for(count = 0 ; count< count_; count++){
			if(Gapw[count] - gapFactor > 0){
				GapLoc.push_back(1);
				wordCount++;
			}else{
				GapLoc.push_back(0);
			}
		}

		int numOfWord = 0;
		int Words_arrSize = wordCount;
		vector<vector<int>> Words;
		vector<int> tempWord;

		int wordStart = Gaps[0][0];
		int wordEnd   = Gaps[0][1];

		for(count = 0 ;count<count_;count++){
			if(GapLoc[count] == 0){
				wordEnd = Gaps[count+1][1];
			}else{
				tempWord.push_back(wordStart);
				tempWord.push_back(wordEnd);
				Words.push_back(tempWord);
				tempWord.clear();

				wordStart = Gaps[count+1][0];
				wordEnd   = Gaps[count+1][1];

				numOfWord = numOfWord + 1;
			}
		}

		tempWord.push_back(wordStart);
		tempWord.push_back(wordEnd);
		Words.push_back(tempWord);
		tempWord.clear();

		for (int numW = 0; numW <= numOfWord; numW++){
			tempWord.push_back(Words[numW][0]);
			tempWord.push_back(Words[numW][1]);
			WordInfo.push_back(tempWord);
			tempWord.clear();
		}
		tempWord.~vector();

		GapLoc.~vector();
		Words.~vector();
	}else{
		vector<int> tempWord;
		tempWord.push_back(Gaps[0][0]);
		tempWord.push_back(Gaps[0][1]);
		WordInfo.push_back(tempWord);
		tempWord.~vector();
	}

	Vhist.~vector();
	Minima.~vector();
	Gapw.~vector();
	Gaps.~vector();

	return WordInfo;
}
vector<vector<int>> HSegmenter::getUnits(int** I, int numRow, int numCol, int start, int end){
	vector<vector<int>> UnitsInfo;
	int wordR = numRow;
	int wordC = end-start+1;
	int** Word = allocate2DIntArray(wordR, wordC);

	for(int row=0; row<numRow; row++){
		for(int col=start, c = 0; col<=end; col++, c++){
			Word[row][c] = I[row][col];
		}
	}				

	int r = numRow;
	int c = end-start+1;

	vector<vector<int> > Units;

	int totalNumberOfUnits = 0;

	int top = 1;
	for(int i=0;i<r;i++){
		if((c - getSum('r', i, Word, wordR, wordC)) != 0){
			top = i;
			break;
		}
	}

	int bottom = r;
	for(int i=r-1;i>=0;i--){
		if((c - getSum('r', i, Word, wordR, wordC)) != 0){
			bottom = i;
			break;
		}
	}

	int** TWord = cropingImage(Word, top, bottom, 0, c-1);

	int r1 = bottom-top+1;
	int c1 = (c-1)-0+1;

	int thVal = 5;

	vector<int> RLH = rlhist(TWord, r1, c1);

	int size = r1;
	int inDx = getMaxIndx(RLH);

	if(r1>thVal && r1<1.5*c1 && inDx<(r1*3)/7){
		int ed_index_up_zone;
		int st_index_ml_zone;

		getMatraaLocPH(TWord, r1, c1, ed_index_up_zone, st_index_ml_zone);
		st_index_ml_zone = st_index_ml_zone + 1;

		int matraa_start_loc;
		int matraa_end_loc;
		if(inDx >= ed_index_up_zone && inDx <= st_index_ml_zone){
			matraa_start_loc = top + ed_index_up_zone + 1;
			matraa_end_loc   = top + st_index_ml_zone - 1;
		}else{
			matraa_start_loc = top + inDx - 1;
			matraa_end_loc   = top + inDx + 1;
		}

		int mWordR = bottom - matraa_start_loc + 1;
		int mWordC =  wordC-1-0+1;
		int** MWord = cropingImage(Word, matraa_start_loc, bottom, 0, wordC-1);

		int upPartR = matraa_end_loc - matraa_start_loc + 1;
		int upPartC =  wordC-1-0+1;
		int** UpPart = cropingImage(Word, matraa_start_loc, matraa_end_loc, 0, wordC-1);

		int** UpEnvelope= allocate2DIntArray(upPartR, upPartC);

		for(int upr=0; upr<upPartR; upr++){
			for(int upc=0; upc<upPartC; upc++){
				UpEnvelope[upr][upc] = 1;
			}
		}

		for(int upc=0; upc<upPartC; upc++){
			for(int upr=0; upr<upPartR; upr++){
				if(UpPart[upr][upc] == 0){
					UpEnvelope[upr][upc] = 0;
					break;
				}
			}
		}

		float thValMatraa = 0.5;
		int numOfRowInUpPart = upPartR;


		/*bwl = gcnew BWLabel(UpEnvelope, upPartR, upPartC);
		int maxlabelNumber = bwl->getNoOfConnectedCom();
		int** LabeledWord = bwl->getLabel();*/

		cca = gcnew CCAnalyzer(UpEnvelope, upPartR, upPartC, false);
		int maxlabelNumber = cca->getNumberOfCC();
		int** LabeledWord = cca->getLabeledArray();
		

		if(maxlabelNumber >= 1){
			for( int numOfCC = 1 ; numOfCC<=maxlabelNumber; numOfCC++){
				vector<point> points = find(numOfCC, LabeledWord, upPartR, upPartC);

				int totalPoints = points.size();
				vector<int> rInfo;
				vector<int> cInfo;

				for(int numOfPoint=0; numOfPoint<totalPoints; numOfPoint++){
					rInfo.push_back(points[numOfPoint].x);
					cInfo.push_back(points[numOfPoint].y);
				}

				int leftCC = getMinVal(cInfo);
				int rightCC = getMaxVal(cInfo);
				int bottomCC = getMaxVal(rInfo);

				int width = rightCC - leftCC + 1;

				float ratioMatraa = float(width) / r1;

				if(ratioMatraa < thValMatraa && bottomCC > ceil((float)numOfRowInUpPart/2) ){
				}else{
					for (int mwi = 0; mwi<numOfRowInUpPart; mwi++){
						for (int mwj = leftCC; mwj<=rightCC; mwj++){
							MWord[mwi][mwj] = 1;
						}
					}
				}

				points.~vector();
				rInfo.~vector();
				cInfo.~vector();
			}
		} 

		int r2 = mWordR;
		int c2 = mWordC;

		vector<int> VH;

		for(int i=0;i<c2;i++){
			VH.push_back(r2 - getSum('c', i, MWord, mWordR, mWordC));
		}

		int count = -1;

		vector<int> CB;

		int startSearchIndex = 0;

		if(VH[0]!=0){
			count = count + 1;
			CB.push_back(-1);
			startSearchIndex = 1;
		}

		for( int i = startSearchIndex;i<c2;i++){
			if(VH[i]==0){
				count = count + 1;
				CB.push_back(i);
			}
		}

		if(VH[c2-1]!=0){
			count = count + 1;
			CB.push_back(c2); 
		}
		
		thVal = 1;
		int count_ = -1;
		int left_, right_, width_;
		int unitTop, unitBottom, height_;
		bool validMZUnit;
		int thHtUnit = (int)ceil((float)r2 / 7);

		vector<vector<int> > PUnits;

		for(int i = 1; i<=count; i++){
			if(CB[i] - CB[i-1] > thVal){
				count_++;

				vector<int> tempUnit;

				left_ = CB[i-1] + 1;
				right_ = CB[i] - 1;
				width_ = right_ - left_ + 1;

				unitTop = top + (ed_index_up_zone + 1);

				for(int botIndx = r2-1 ; botIndx>=0 ; botIndx--){
					int tempWd = getSumLR(MWord, botIndx, left_, right_);

					if(tempWd != width_){
						unitBottom = bottom - (r2 - botIndx) + 1;
						break;
					}
				}

				height_ = unitBottom - unitTop + 1;

				validMZUnit = true;

				if(height_ <thHtUnit)
					validMZUnit = false;

				if(validMZUnit){ 

					tempUnit.push_back(unitTop);
					tempUnit.push_back(unitBottom);
					tempUnit.push_back(left_);
					tempUnit.push_back(right_);
					tempUnit.push_back(height_);
					tempUnit.push_back(width_);
					tempUnit.push_back(matraa_start_loc);
					tempUnit.push_back(matraa_end_loc);
					tempUnit.push_back(2);
					PUnits.push_back(tempUnit);
					tempUnit.~vector();
				}
			}
		}
		
		// if the units are not correctly determined then free memory and return
		if(PUnits.size() < 1){
			// keep the unit as it is; mark this as upper zone unit
			vector<int> tempUnit;

			tempUnit.push_back(unitTop);
			tempUnit.push_back(unitBottom);
			tempUnit.push_back(left_);
			tempUnit.push_back(right_);
			tempUnit.push_back(height_);
			tempUnit.push_back(width_);
			tempUnit.push_back(1);
			tempUnit.push_back(1);
			tempUnit.push_back(2);
			UnitsInfo.push_back(tempUnit);
			tempUnit.~vector();

			// free memory
			delete2DIntArray(MWord, mWordR);
			delete2DIntArray(UpPart, upPartR);
			delete2DIntArray(UpEnvelope, upPartR);
			delete2DIntArray(LabeledWord, upPartR);	
			delete2DIntArray(Word, wordR);
			delete2DIntArray(TWord, r1);
			CB.~vector();
			VH.~vector();
			RLH.~vector();
			PUnits.~vector();
			Units.~vector();

			return UnitsInfo;
		}

		PUnits = eliminateMBJError(PUnits, Word);
		Units = eliminateSplittingError(PUnits);
		
		PUnits.~vector();

		count_ = Units.size() - 1;
		totalNumberOfUnits = count_;

		vector<int> tempArr;

		for(int tmpCnt = 0; tmpCnt<=count_; tmpCnt++)
			tempArr.push_back(Units[tmpCnt][4]);

		int thHtUpUnit = (int)ceil(getMean(tempArr) / 6);
		int matraaLength = st_index_ml_zone - ed_index_up_zone - 1;

		if(ed_index_up_zone+1 >= matraaLength){
			int** UZWord = cropingImage(TWord, 0,ed_index_up_zone,0,c1-1);

			r2 = ed_index_up_zone - 0 + 1;
			c2 = (c1-1) - 0 + 1;

			vector<int> HH;

			for(int i=0; i<r2; i++){
				HH.push_back(c2 -getSum('r', i, UZWord, r2, c2)); 
			}

			int inDx = getMaxIndx(HH);

			HH.~vector();

			if(inDx >= r2 - ceil((float)r2/8)){
				ed_index_up_zone = inDx - 1;

				for(int i=inDx; i<r2; i++){
					for(int j=0; j<c2; j++){
						UZWord[i][j] = 1;
					}
				}
			}

			VH.clear();
			for(int  i = 0;i<c2;i++){
				VH.push_back(r2 - getSum('c',i,UZWord, r2, c2)); 
			}

			count = -1;

			startSearchIndex = 0; 
			CB.clear();
			if(VH[0]!=0){
				count = count + 1;
				CB.push_back(-1); 
				startSearchIndex = 1;
			}

			for(int i = startSearchIndex; i<c2; i++){
				if(VH[i]==0){
					count = count + 1;
					CB.push_back(i); 
				}
			}

			if(VH[c2-1]!=0){
				count = count + 1;
				CB.push_back(c2); 
			}

			thVal = 1;
			bool upperZonePresent;
			int topUnit, pixelCount;
			float thValTopUnit = 0.5;
			float ratioTopUnit;
			for(int i = 1;i<=count;i++){
				if(CB[i] - CB[i-1] > thVal){						 
					upperZonePresent = true;

					left_ = CB[i-1]+1;
					right_ = CB[i]-1;
					width_ = right_ - left_ + 1;

					topUnit = r2;

					for(int tpIndx=0; tpIndx<r2; tpIndx++){
						pixelCount = width_ - getSumLR(UZWord, tpIndx, left_, right_);
						if(pixelCount != 0){
							topUnit = tpIndx;
							break;
						}
					}

					height_ = ed_index_up_zone - topUnit + 1;

					ratioTopUnit = (float)(topUnit+1)/r2;

					if(ratioTopUnit > thValTopUnit){
						upperZonePresent = false;
					}

					if(height_ < matraaLength || height_ < thHtUpUnit){
						upperZonePresent = false;
					}

					if(upperZonePresent){					
						vector<int> tempUnit;

						tempUnit.push_back(topUnit);
						tempUnit.push_back(top + ed_index_up_zone);
						tempUnit.push_back(left_);
						tempUnit.push_back(right_);
						tempUnit.push_back(height_);
						tempUnit.push_back(width_);
						tempUnit.push_back(-1);
						tempUnit.push_back(-1);
						tempUnit.push_back(1);
						Units.push_back(tempUnit);
						tempUnit.~vector();
					}
				}
			}

			delete2DIntArray(UZWord, r2);
		}

		totalNumberOfUnits = Units.size();

		delete2DIntArray(MWord, mWordR);
		delete2DIntArray(UpPart, upPartR);
		delete2DIntArray(UpEnvelope, upPartR);
		delete2DIntArray(LabeledWord, upPartR);	

		// free memory
		VH.~vector();
		CB.~vector();
		tempArr.~vector();
	}
	else{
		vector<int> VH;

		if(r1 < 2*c1){
			for(int i = 0;i<c1;i++){
				VH.push_back(r1 - getSum('c',i,TWord, r1, c1));
			}

			int count = -1;

			int startSearchIndex = 0;

			vector<int> CB;

			if(VH[0]!=0){
				count = count + 1;
				CB.push_back(-1); 
				startSearchIndex = 1;
			}

			for(int i=startSearchIndex; i<c1; i++){
				if(VH[i]==0){
					count = count + 1;
					CB.push_back(i); 
				}
			}

			if(VH[c1-1]!=0){
				count = count + 1;
				CB.push_back(c1);
			}

			int thVal = 1;
			int count_ = -1;
			int left_, right_, width_;

			for(int i = 1; i<=count; i++){
				if(CB[i] - CB[i-1] > thVal){
					count_++;

					vector<int> tempUnit;

					left_ = CB[i-1] + 1;
					right_ = CB[i] - 1;
					width_ = right_ - left_ + 1;

					tempUnit.push_back(top);

					for(int botIndx = r1-1; botIndx>=0;botIndx--){
						int tempWd = getSumLR(TWord, botIndx, left_, right_);

						if(tempWd != width_){
							tempUnit.push_back(bottom - (r1 - botIndx) + 1);
							break;
						}
					}

					tempUnit.push_back(left_);
					tempUnit.push_back(right_);						
					tempUnit.push_back(tempUnit[1] - tempUnit[0] + 1);
					tempUnit.push_back(width_);
					tempUnit.push_back(top); 
					tempUnit.push_back(top); 
					tempUnit.push_back(2);
					Units.push_back(tempUnit);

					tempUnit.~vector();
				}
			}

			totalNumberOfUnits = count_;

			CB.~vector();
		}
		else{
			vector<int> tempUnit;

			tempUnit.push_back(top);
			tempUnit.push_back(bottom);
			tempUnit.push_back(0);
			tempUnit.push_back(c-1);
			tempUnit.push_back(tempUnit[1] - tempUnit[0] + 1);
			tempUnit.push_back(tempUnit[3] - tempUnit[2] + 1);
			tempUnit.push_back(top); 
			tempUnit.push_back(top); 
			tempUnit.push_back(2);				
			Units.push_back(tempUnit);

			tempUnit.~vector();

			totalNumberOfUnits = 0;
		}

		VH.~vector();
	}

	this->numOfElementsPerUnit = 9;
	
	vector<int> perUnitsInfo;

	int sizeOfUnits = Units.size();
	for (int numUnit = 0; numUnit < sizeOfUnits; numUnit++){
		perUnitsInfo.push_back(Units[numUnit][0]);
		perUnitsInfo.push_back(Units[numUnit][1]);
		perUnitsInfo.push_back(Units[numUnit][2]);
		perUnitsInfo.push_back(Units[numUnit][3]);
		perUnitsInfo.push_back(Units[numUnit][4]);
		perUnitsInfo.push_back(Units[numUnit][5]);
		perUnitsInfo.push_back(Units[numUnit][6]);
		perUnitsInfo.push_back(Units[numUnit][7]);
		perUnitsInfo.push_back(Units[numUnit][8]);
		UnitsInfo.push_back(perUnitsInfo);
		perUnitsInfo.clear();
	}

	// delete Word
	RLH.~vector();
	delete2DIntArray(Word, wordR);
	delete2DIntArray(TWord, r1);
	Units.~vector();

	return UnitsInfo;
}

vector<vector<int> > HSegmenter::eliminateMBJError(vector<vector<int> > PUnits, int** Word){
	vector<vector<int> > Units;

	int numOfUnit = PUnits.size();
	float aspRatio;
	int r, c, totalPixel;
	float thVal, thValLastMatraaRow;
	bool changeStatus;
	int** TI;

	for(int num=0; num<numOfUnit; num++){
		aspRatio = (float)PUnits[num][5] / PUnits[num][4];

		if(aspRatio > 1.2){
			TI = cropingImage(Word, PUnits[num][7], PUnits[num][1], PUnits[num][2], PUnits[num][3]);

			r = PUnits[num][1] - PUnits[num][7] + 1;
			c = PUnits[num][3] - PUnits[num][2] + 1;

			thVal = c * 0.75f;
			thValLastMatraaRow = c * 0.65f;
			changeStatus = false;

			for(int i=0; i<ceil((float)r/4); i++){
				totalPixel = c - getSum('r', i, TI, r, c);
				if(totalPixel >= thVal){
					for(int j=0; j<c; j++){
						TI[i][j] = 1;
					}

					changeStatus = true;
				}
				else if(totalPixel >= thValLastMatraaRow){ 
					for(int j=0; j<c; j++){
						TI[i][j] = 1;
					}

					changeStatus = true;
					break;
				}
				else{
					break;
				}
			}

			if(changeStatus){
				vector<int> VH;

				for(int i=0; i<c; i++){
					VH.push_back(r - getSum('c', i, TI, r, c)); 
				}

				int count = -1;

				vector<int> CB;

				int startSearchIndex = 0;

				if(VH[0]!=0){
					count++;
					CB.push_back(-1); 
					startSearchIndex = 1;
				}

				for( int i = startSearchIndex;i<c;i++){
					if(VH[i]==0){
						count++;
						CB.push_back(i); 
					}
				}

				if(VH[c-1]!=0){
					count++;
					CB.push_back(c); 
				}

				thVal = 1;
				int left_, right_, width_;

				for(int i = 1; i<=count; i++){
					if(CB[i] - CB[i-1] > thVal){
						vector<int> tempUnit;

						left_ = CB[i-1] + 1;
						right_ = CB[i] - 1;
						width_ = right_ - left_ + 1;

						tempUnit.push_back(PUnits[num][0]);
						tempUnit.push_back(PUnits[num][1]);
						tempUnit.push_back(PUnits[num][2] + left_ - 1);
						tempUnit.push_back(PUnits[num][2] + right_ - 1);
						tempUnit.push_back(PUnits[num][4]);
						tempUnit.push_back(width_);
						tempUnit.push_back(PUnits[num][6]);
						tempUnit.push_back(PUnits[num][7]);
						tempUnit.push_back(2);

						Units.push_back(tempUnit);

						tempUnit.~vector();
					}
				}

				VH.~vector();
				CB.~vector();
			}else{ 	
				vector<int> tempUnit;

				tempUnit.push_back(PUnits[num][0]);
				tempUnit.push_back(PUnits[num][1]);
				tempUnit.push_back(PUnits[num][2]);
				tempUnit.push_back(PUnits[num][3]);
				tempUnit.push_back(PUnits[num][4]);
				tempUnit.push_back(PUnits[num][5]);
				tempUnit.push_back(PUnits[num][6]);
				tempUnit.push_back(PUnits[num][7]);
				tempUnit.push_back(PUnits[num][8]);

				Units.push_back(tempUnit);

				tempUnit.~vector();
			}

			delete2DIntArray(TI, r);
		}else{ 

			vector<int> tempUnit;

			tempUnit.push_back(PUnits[num][0]);
			tempUnit.push_back(PUnits[num][1]);
			tempUnit.push_back(PUnits[num][2]);
			tempUnit.push_back(PUnits[num][3]);
			tempUnit.push_back(PUnits[num][4]);
			tempUnit.push_back(PUnits[num][5]);
			tempUnit.push_back(PUnits[num][6]);
			tempUnit.push_back(PUnits[num][7]);
			tempUnit.push_back(PUnits[num][8]);

			Units.push_back(tempUnit);

			tempUnit.~vector();
		} 
	}

	return Units;
}

vector<vector<int> > HSegmenter::eliminateSplittingError(vector<vector<int> > PUnits){
	vector<vector<int> > Units;

	int pUnitSize = PUnits.size();

	vector<int> tempArray;

	for(int tmp=0; tmp<pUnitSize; tmp++){
		tempArray.push_back(PUnits[tmp][4]);
	} 

	float avgPUnitsBottom = getMean(tempArray);
	float medianPUnitsBottom = getMedian(tempArray);
	float stdPUnitsBottom = getStdDev(tempArray);
	int maxPUnitsBottom = getMaxVal(tempArray);

	// free memory
	tempArray.~vector();

	float thUnitBtLoc1 = 0;
	float thUnitBtLoc2, thUnitBtLoc2_1, thUnitBtLoc2_2;

	if(maxPUnitsBottom - medianPUnitsBottom > stdPUnitsBottom){
		thUnitBtLoc2_1 = avgPUnitsBottom - stdPUnitsBottom;
	}else{
		thUnitBtLoc2_1 = maxPUnitsBottom - (avgPUnitsBottom/5 + stdPUnitsBottom);
	}

	thUnitBtLoc2_2 = maxPUnitsBottom - (avgPUnitsBottom/5 + stdPUnitsBottom);

	if(thUnitBtLoc2_1 < thUnitBtLoc2_2){
		thUnitBtLoc2 = ceil(thUnitBtLoc2_1);
	}else{
		thUnitBtLoc2 = ceil(thUnitBtLoc2_2);
	}

	float thUnitWdHtRatio = 0.4f;
	float thUnitHtHtRatio = 0.75f;

	int unitsCount = -1;

	int numOfPUnits = pUnitSize;
	int startOfUnit;
	float ratioOfWdHtNextUnit, zValue;

	if(numOfPUnits > 1){
		startOfUnit = PUnits[0][2];

		for(int inDxCount = 0; inDxCount<numOfPUnits-1; inDxCount++){
			ratioOfWdHtNextUnit = (float)PUnits[inDxCount+1][5] / PUnits[inDxCount+1][4];
			zValue = (float)(PUnits[inDxCount][4] - avgPUnitsBottom ) / ((float)stdPUnitsBottom + 0.001f);
			if((zValue < thUnitBtLoc1) && (PUnits[inDxCount][4] <= thUnitBtLoc2) && (ratioOfWdHtNextUnit < thUnitWdHtRatio) && ((float)PUnits[inDxCount][4] / PUnits[inDxCount+1][4] < thUnitHtHtRatio)){
				startOfUnit = PUnits[inDxCount][2];
			}else{
				unitsCount++;

				vector<int> tempUnit;

				tempUnit.push_back(PUnits[inDxCount][0]);
				tempUnit.push_back(PUnits[inDxCount][1]);
				tempUnit.push_back(startOfUnit);
				tempUnit.push_back(PUnits[inDxCount][3]);
				tempUnit.push_back(PUnits[inDxCount][4]);
				tempUnit.push_back(tempUnit[3] - tempUnit[2] + 1);
				tempUnit.push_back(PUnits[inDxCount][6]);
				tempUnit.push_back(PUnits[inDxCount][7]);
				tempUnit.push_back(PUnits[inDxCount][8]);

				Units.push_back(tempUnit);
				tempUnit.~vector();

				startOfUnit = PUnits[inDxCount+1][2];
			}
		}

		unitsCount++;

		vector<int> tempUnit;

		tempUnit.push_back(PUnits[numOfPUnits-1][0]);
		tempUnit.push_back(PUnits[numOfPUnits-1][1]);
		tempUnit.push_back(startOfUnit);
		tempUnit.push_back(PUnits[numOfPUnits-1][3]);
		tempUnit.push_back(PUnits[numOfPUnits-1][4]);
		tempUnit.push_back(tempUnit[3] - tempUnit[2] + 1);
		tempUnit.push_back(PUnits[numOfPUnits-1][6]);
		tempUnit.push_back(PUnits[numOfPUnits-1][7]);
		tempUnit.push_back(PUnits[numOfPUnits-1][8]);

		Units.push_back(tempUnit);
		tempUnit.~vector();

		return Units;
	}else{
		return PUnits;
	}
}	


vector<UnitInformation> HSegmenter::getJoiningErrorFreeUnits(int** I, vector<UnitInformation> LineWordsInfo){
	vector<UnitInformation> LineUnitsInfo;

	int LineUnitsCount = -1;
	int totalUnits = LineWordsInfo.size();

	vector<vector<int>> MZUnits;
	vector<int> singleMZUnit;
	
	int mzUnitCount = -1; 
	for(int tempCounter = 0; tempCounter<totalUnits; tempCounter++){
		if(LineWordsInfo[tempCounter].zoneNumber == 2){
			mzUnitCount++;
			singleMZUnit.push_back(tempCounter);
			singleMZUnit.push_back(LineWordsInfo[tempCounter].width);
			MZUnits.push_back(singleMZUnit);
			singleMZUnit.clear();
		}
	}
	singleMZUnit.~vector();

	vector<int> wdContainer =  copyDesiredColumn(MZUnits, 1);

	int tempWdCounter;

	if(mzUnitCount+1 <= 5)
		tempWdCounter = mzUnitCount+1;
	else
		tempWdCounter = 5;

	vector<int> tempWdContainer;

	int tempCount = -1;
	int maxWd, tempWdIndx;
	float wdVsHtRatio; 

	while(tempCount < tempWdCounter-1){
		maxWd = getMaxVal(wdContainer);
		tempWdIndx = getMaxIndx(wdContainer);
		wdVsHtRatio = (float) maxWd / LineWordsInfo[MZUnits[tempWdIndx][0]].height;

		if(wdVsHtRatio < 2){
			tempCount++;
			tempWdContainer.push_back(maxWd);
			wdContainer[tempWdIndx] = 0;
		}else{
			wdContainer[tempWdIndx] = maxWd / wdVsHtRatio;
		}
	}

	int maxCharWd = getMedian(tempWdContainer);

	tempWdContainer.~vector();
	wdContainer.~vector();

	float wbin1Th = maxCharWd * 0.8f;   
	float wbin2Th = maxCharWd * 0.64f;  

	int wbin1Sz = -1; 
	int wbin2Sz = -1; 
	int wbin3Sz = -1; 

	int wdUnit;
	vector<vector<int>> wBin1;
	vector<vector<int>> wBin2;
	vector<vector<int>> wBin3;
	vector<int> tmpBinElement;

	for (int chrIndx = 0; chrIndx<=mzUnitCount; chrIndx++){
		wdUnit = MZUnits[chrIndx][1];

		if(wdUnit >= wbin1Th){
			wbin1Sz++;
			tmpBinElement.push_back(MZUnits[chrIndx][0]);
			tmpBinElement.push_back(wdUnit);	
			wBin1.push_back(tmpBinElement);  
			tmpBinElement.clear();
		}else if(wdUnit < wbin1Th && wdUnit >= wbin2Th){
			wbin2Sz++;
			tmpBinElement.push_back(MZUnits[chrIndx][0]);
			tmpBinElement.push_back(wdUnit);	
			wBin2.push_back(tmpBinElement);  
			tmpBinElement.clear();
		}else{
			wbin3Sz++;
			tmpBinElement.push_back(MZUnits[chrIndx][0]);
			tmpBinElement.push_back(wdUnit);	
			wBin3.push_back(tmpBinElement);  
			tmpBinElement.clear();
		}
	}
	MZUnits.~vector();

	int mBinCount = -1;
	int mBinSize = wbin1Sz + 1 + wbin2Sz + 1;
	vector<vector<int>> tempMBin;

	int wbin1Count = -1;
	if(wbin1Sz > -1){
		while(wbin1Count < wbin1Sz){
			mBinCount++;
			wbin1Count++;

			tmpBinElement.push_back(wBin1[wbin1Count][0]);
			tmpBinElement.push_back(wBin1[wbin1Count][1]);	
			tempMBin.push_back(tmpBinElement);  
			tmpBinElement.clear();
		}
	}

	int wbin2Count = -1;
	if(wbin2Sz > -1){
		while(wbin2Count < wbin2Sz){
			mBinCount++;
			wbin2Count++;

			tmpBinElement.push_back(wBin2[wbin2Count][0]);
			tmpBinElement.push_back(wBin2[wbin2Count][1]);	
			tempMBin.push_back(tmpBinElement);  
			tmpBinElement.clear();
		}
	}
	tmpBinElement.~vector();

	vector<vector<int>> MBin;
	
	MBin = sortByRows(tempMBin, 2, 0);

	int avgWd;

	if(wbin2Count >= 0){
		vector<int> tempArr = copyDesiredColumn(wBin2, 1);

		avgWd = floor(getMean(tempArr));
		tempArr.~vector();
	}
	else{
		vector<int> tempArr = copyDesiredColumn(wBin1, 1);

		avgWd = floor(getMean(tempArr));
		tempArr.~vector();
	}

	if(avgWd < 3){
		avgWd = 3;
	}

	wBin1.~vector();
	wBin2.~vector();
	wBin3.~vector();
	tempMBin.~vector();

	int segmentedUnitsCount = -1;

	vector<vector<int>> segmentedUnitsInfo;

	tempCount = -1; 
	int thValHt = 5;
	int wdOfUnit;
	int imgIndx, unitTop, unitBottom, unitLeft, unitRight, unitHeight;

	for(int mbinIndx = 0; mbinIndx<=mBinCount; mbinIndx++){				
		wdOfUnit = MBin[mbinIndx][1];

		if(wdOfUnit >= avgWd && LineWordsInfo[MBin[mbinIndx][0]].height >= thValHt){
			imgIndx = MBin[mbinIndx][0];

			unitTop = LineWordsInfo[imgIndx].matraaEndLoc;
			unitBottom = LineWordsInfo[imgIndx].bottom;
			unitLeft = LineWordsInfo[imgIndx].left;
			unitRight = LineWordsInfo[imgIndx].right;
			unitHeight = unitBottom - unitTop + 1;

			int sIR = unitBottom - unitTop + 1;
			int sIC = unitRight - unitLeft + 1;
			int** SI = cropingImage(I, unitTop, unitBottom, unitLeft, unitRight);

			//bwl = gcnew BWLabel(SI, sIR, sIC);
			//int numcc = bwl->getNoOfConnectedCom();

			cca = gcnew CCAnalyzer(SI, sIR, sIC, false);
			int numcc = cca->getNumberOfCC();

			if(numcc > 1){	
				vector<vector<int>> ConnComp;
				vector<int> componentInfo;
				vector<int> rInfo;
				vector<int> cInfo;
				int totalPoints;
				//int** LabeledImage = bwl->getLabel();
				int** LabeledImage = cca->getLabeledArray();

				for (int numOfCC = 0; numOfCC<numcc; numOfCC++){
					vector<point> points = find(numOfCC+1, LabeledImage, sIR, sIC);

					totalPoints = points.size();

					for(int numOfPoint=0; numOfPoint<totalPoints; numOfPoint++){
						rInfo.push_back(points[numOfPoint].x);
						cInfo.push_back(points[numOfPoint].y);
					}

					componentInfo.push_back(getMinVal(rInfo));
					componentInfo.push_back(getMaxVal(rInfo));
					componentInfo.push_back(getMinVal(cInfo));
					componentInfo.push_back(getMaxVal(cInfo));
					componentInfo.push_back(componentInfo[1] - componentInfo[0] + 1);
					componentInfo.push_back(componentInfo[3] - componentInfo[2] + 1);
					ConnComp.push_back(componentInfo);
					componentInfo.clear();


					points.~vector();
					rInfo.~vector();
					cInfo.~vector();
				}
				componentInfo.~vector();
				vector<vector<int>> CCInfo = sortByRows(ConnComp, 6, 2);

				// free memory
				delete2DIntArray(LabeledImage, sIR);
				ConnComp.~vector();

				int thVal = 3;

				segmentedUnitsCount++;

				LineWordsInfo[imgIndx].locOfShadowUnit = segmentedUnitsCount;

				int prevTempCount = tempCount;

				int olapBTNCC, wdOfCC, topOfUnit, bottomOfUnit, leftOfUnit, rightOfUnit;

				for (int numOfCC = 0; numOfCC<numcc-1; numOfCC++){
					olapBTNCC = CCInfo[numOfCC][3] - CCInfo[numOfCC+1][2] + 1;
					wdOfCC = CCInfo[numOfCC+1][3] - CCInfo[numOfCC][2] + 1;

					if ((olapBTNCC  <= thVal) && (CCInfo[numOfCC+1][0] < CCInfo[numOfCC+1][1]/2) && (wdOfCC >= thVal) && (CCInfo[numOfCC][1]> unitHeight/2)){
						topOfUnit = LineWordsInfo[imgIndx].matraaSrartLoc;
						bottomOfUnit = unitTop + CCInfo[numOfCC][1];
						leftOfUnit = unitLeft + CCInfo[numOfCC][2];
						rightOfUnit = unitLeft + CCInfo[numOfCC][3];
						tempCount++;

						vector<int> tempUnit;
						tempUnit.push_back(segmentedUnitsCount);
						tempUnit.push_back(topOfUnit);
						tempUnit.push_back(bottomOfUnit);
						tempUnit.push_back(leftOfUnit);
						tempUnit.push_back(rightOfUnit);

						segmentedUnitsInfo.push_back(tempUnit);

						tempUnit.~vector();
					}else{
						if(CCInfo[numOfCC][0] < CCInfo[numOfCC+1][0])
							CCInfo[numOfCC+1][0] = CCInfo[numOfCC][0];

						if(CCInfo[numOfCC][1] > CCInfo[numOfCC+1][1])
							CCInfo[numOfCC+1][1] = CCInfo[numOfCC][1];

						if(CCInfo[numOfCC][2] < CCInfo[numOfCC+1][2])
							CCInfo[numOfCC+1][2] = CCInfo[numOfCC][2];

						if(CCInfo[numOfCC][3] > CCInfo[numOfCC+1][3])
							CCInfo[numOfCC+1][3] = CCInfo[numOfCC][3];
					}
				}

				topOfUnit = LineWordsInfo[imgIndx].matraaSrartLoc;
				bottomOfUnit = unitTop + CCInfo[numcc-1][1];
				leftOfUnit = unitLeft + CCInfo[numcc-1][2];
				rightOfUnit = unitLeft + CCInfo[numcc-1][3];
				if(prevTempCount == tempCount){
					int ht = sIR; 
					int wd = sIC;

					if((float)ht/wd <= 0.8){
						topOfUnit = LineWordsInfo[imgIndx].matraaSrartLoc;
						bottomOfUnit = LineWordsInfo[imgIndx].bottom;

						vector<int> TPoints;

						if(TPoints.size() > 0){
							vector<vector<int> > Units = getUnitsFromTouchingPoints(TPoints, SI, sIR, sIC);

							int numberOfUnit = Units.size();

							for(int printUnitCount = 0; printUnitCount<numberOfUnit; printUnitCount++){
								vector<int> tempUnit;

								tempUnit.push_back(segmentedUnitsCount);
								tempUnit.push_back(topOfUnit);
								tempUnit.push_back(bottomOfUnit);
								tempUnit.push_back(unitLeft + Units[printUnitCount][0] - 1);
								tempUnit.push_back(unitLeft + Units[printUnitCount][1] - 1);

								segmentedUnitsInfo.push_back(tempUnit);

								tempUnit.~vector();
							}

							Units.~vector();
						}
						else{ 
							vector<int> tempUnit;
							tempUnit.push_back(segmentedUnitsCount);
							tempUnit.push_back(topOfUnit);
							tempUnit.push_back(bottomOfUnit);
							tempUnit.push_back(leftOfUnit);
							tempUnit.push_back(rightOfUnit);

							segmentedUnitsInfo.push_back(tempUnit);

							tempUnit.~vector();
						}

						TPoints.~vector();
					}else{
						tempCount++;
						vector<int> tempUnit;
						tempUnit.push_back(segmentedUnitsCount);
						tempUnit.push_back(topOfUnit);
						tempUnit.push_back(bottomOfUnit);
						tempUnit.push_back(leftOfUnit);
						tempUnit.push_back(rightOfUnit);

						segmentedUnitsInfo.push_back(tempUnit);
						tempUnit.~vector();
					} 
				}else{
					tempCount++;
					vector<int> tempUnit;

					tempUnit.push_back(segmentedUnitsCount);
					tempUnit.push_back(topOfUnit);
					tempUnit.push_back(bottomOfUnit);
					tempUnit.push_back(leftOfUnit);
					tempUnit.push_back(rightOfUnit);

					segmentedUnitsInfo.push_back(tempUnit);
					tempUnit.~vector();
				} 

				CCInfo.~vector();
			}else{  
				int ht = sIR; 
				int wd = sIC;

				if((float)ht/wd <= 0.8){
					vector<int> TPoints = eliminateTouchingError(SI, sIR, sIC);

					if(!TPoints.empty()){
						vector<vector<int> > Units = getUnitsFromTouchingPoints(TPoints, SI, sIR, sIC);

						int numberOfUnit = Units.size();

						if(numberOfUnit > 1){
							segmentedUnitsCount++;
							LineWordsInfo[imgIndx].locOfShadowUnit = segmentedUnitsCount;

							int topOfUnit = LineWordsInfo[imgIndx].matraaSrartLoc - 1;
							int bottomOfUnit = LineWordsInfo[imgIndx].bottom;

							for(int printUnitCount = 0; printUnitCount<numberOfUnit; printUnitCount++){
								vector<int> tempUnit;
								tempUnit.push_back(segmentedUnitsCount);
								tempUnit.push_back(topOfUnit);
								tempUnit.push_back(bottomOfUnit);
								tempUnit.push_back(unitLeft + Units[printUnitCount][0] - 1);
								tempUnit.push_back(unitLeft + Units[printUnitCount][1] - 1);

								segmentedUnitsInfo.push_back(tempUnit);

								tempUnit.~vector();
							}
						}

						Units.~vector();
					}

					TPoints.~vector();
				}  				
			} 

			delete2DIntArray(SI, sIR);
		}
	}

	MBin.~vector();

	if (!segmentedUnitsInfo.empty()){
		int numOfLineWordUnits = LineWordsInfo.size();

		int numOfSegUnits = segmentedUnitsInfo.size();

		int tempIndex = 0;
		int indexSegUnit;

		for(int LineWordsCount = 0; LineWordsCount<numOfLineWordUnits; LineWordsCount++){
			indexSegUnit = LineWordsInfo[LineWordsCount].locOfShadowUnit;

			if(indexSegUnit != -1){
				int countSegShUnit = 0;

				for(int searchIndex = tempIndex; searchIndex<numOfSegUnits; searchIndex++){
					if(segmentedUnitsInfo[searchIndex][0] == indexSegUnit){
						countSegShUnit++;
						LineUnitsCount++;
						UnitInformation uif;
						uif.lineNumber = LineWordsInfo[LineWordsCount].lineNumber;
						uif.wordNumber = LineWordsInfo[LineWordsCount].wordNumber;
						uif.unitNumber = LineWordsInfo[LineWordsCount].unitNumber;
						uif.subUnitNumber = countSegShUnit;
						uif.top = segmentedUnitsInfo[searchIndex][1];
						uif.bottom = segmentedUnitsInfo[searchIndex][2];
						uif.left = segmentedUnitsInfo[searchIndex][3];
						uif.right = segmentedUnitsInfo[searchIndex][4];
						uif.height = uif.bottom - uif.top + 1;
						uif.width = uif.right - uif.left + 1;
						uif.zoneNumber = 2;

						LineUnitsInfo.push_back(uif);
					}
				}

				tempIndex = tempIndex + countSegShUnit;
			}else{
				LineUnitsCount++;
				UnitInformation uif;
				uif.lineNumber = LineWordsInfo[LineWordsCount].lineNumber;
				uif.wordNumber = LineWordsInfo[LineWordsCount].wordNumber;
				uif.unitNumber = LineWordsInfo[LineWordsCount].unitNumber;
				uif.subUnitNumber = 0;
				uif.top = LineWordsInfo[LineWordsCount].top;
				uif.bottom = LineWordsInfo[LineWordsCount].bottom;
				uif.left = LineWordsInfo[LineWordsCount].left;
				uif.right = LineWordsInfo[LineWordsCount].right;
				uif.height = LineWordsInfo[LineWordsCount].height;
				uif.width = LineWordsInfo[LineWordsCount].width;
				uif.zoneNumber = LineWordsInfo[LineWordsCount].zoneNumber;

				LineUnitsInfo.push_back(uif);
			}
		}
	}else{
		int numOfLineWordUnits = LineWordsInfo.size();

		for(int LineWordsCount = 0 ;LineWordsCount<numOfLineWordUnits; LineWordsCount++){
			LineUnitsCount++;

			UnitInformation uif;
			uif.lineNumber = LineWordsInfo[LineWordsCount].lineNumber;
			uif.wordNumber = LineWordsInfo[LineWordsCount].wordNumber;
			uif.unitNumber = LineWordsInfo[LineWordsCount].unitNumber;
			uif.subUnitNumber = 0;
			uif.top = LineWordsInfo[LineWordsCount].top;
			uif.bottom = LineWordsInfo[LineWordsCount].bottom;
			uif.left = LineWordsInfo[LineWordsCount].left;
			uif.right = LineWordsInfo[LineWordsCount].right;
			uif.height = LineWordsInfo[LineWordsCount].height;
			uif.width = LineWordsInfo[LineWordsCount].width;
			uif.zoneNumber = LineWordsInfo[LineWordsCount].zoneNumber;

			LineUnitsInfo.push_back(uif);
		}
	}

	segmentedUnitsInfo.~vector();

	return LineUnitsInfo;
}

vector<UnitInformation> HSegmenter::getSplittingErrorFreeUnitsOfALine(vector<UnitInformation> LineUnitsInfo){
	vector<UnitInformation> SEFLineUnitsInfo;

	int numOfLineUnits = LineUnitsInfo.size();

	int lineNumber = LineUnitsInfo[numOfLineUnits-1].lineNumber;

	int totalWord = LineUnitsInfo[numOfLineUnits-1].wordNumber;

	int unitCounter = -1;
	int wordNumber = -1;
	int searchStartIndex = 0;


	while(unitCounter < numOfLineUnits-1){
		wordNumber++;
		int tempMZUCounter = -1;
		int tempUZUCounter = -1;

		vector<vector<int> > MZUInfo;
		vector<vector<int> > UZUInfo;

		for(int inDx = searchStartIndex; inDx<numOfLineUnits; inDx++){
			if(LineUnitsInfo[inDx].wordNumber == wordNumber){
				unitCounter++;

				if(LineUnitsInfo[inDx].zoneNumber == 2){
					tempMZUCounter++;

					vector<int> tempUnit;
					tempUnit.push_back(LineUnitsInfo[unitCounter].top);
					tempUnit.push_back(LineUnitsInfo[unitCounter].bottom);
					tempUnit.push_back(LineUnitsInfo[unitCounter].left);
					tempUnit.push_back(LineUnitsInfo[unitCounter].right);
					tempUnit.push_back(LineUnitsInfo[unitCounter].height);
					tempUnit.push_back(LineUnitsInfo[unitCounter].width);
					tempUnit.push_back(2);
					tempUnit.push_back(-1);
					tempUnit.push_back(-1);

					MZUInfo.push_back(tempUnit);
					tempUnit.~vector();
				}else{
					tempUZUCounter++;

					vector<int> tempUnit;
					tempUnit.push_back(LineUnitsInfo[unitCounter].top);
					tempUnit.push_back(LineUnitsInfo[unitCounter].bottom);
					tempUnit.push_back(LineUnitsInfo[unitCounter].left);
					tempUnit.push_back(LineUnitsInfo[unitCounter].right);
					tempUnit.push_back(LineUnitsInfo[unitCounter].height);
					tempUnit.push_back(LineUnitsInfo[unitCounter].width);
					tempUnit.push_back(1);

					UZUInfo.push_back(tempUnit);
					tempUnit.~vector();
				}
			}else{
				searchStartIndex = searchStartIndex + tempMZUCounter+1 + tempUZUCounter+1;
				break;
			}
		}

		vector<vector<int> > MZInfo = eliminateSplittingError(MZUInfo);

		int count = MZInfo.size() - 1;

		int startIndex;
		int endIndex;

		if(count == tempMZUCounter){
			if(wordNumber == totalWord){
				startIndex = searchStartIndex;
				endIndex   = numOfLineUnits-1;
			}else{
				startIndex = searchStartIndex - (MZUInfo.size() + UZUInfo.size());
				endIndex   = searchStartIndex - 1;
			}

			int tempUnitCount = -1;

			for(int wordCopyIndx = startIndex ; wordCopyIndx<=endIndex; wordCopyIndx++){

				UnitInformation uif;

				uif.lineNumber = lineNumber;
				uif.wordNumber = wordNumber;
				tempUnitCount++;
				uif.unitNumber = tempUnitCount;
				uif.top = LineUnitsInfo[wordCopyIndx].top;
				uif.bottom = LineUnitsInfo[wordCopyIndx].bottom;
				uif.left = LineUnitsInfo[wordCopyIndx].left;
				uif.right = LineUnitsInfo[wordCopyIndx].right;
				uif.height = LineUnitsInfo[wordCopyIndx].height;
				uif.width = LineUnitsInfo[wordCopyIndx].width;
				uif.zoneNumber = LineUnitsInfo[wordCopyIndx].zoneNumber;
				SEFLineUnitsInfo.push_back(uif);
			}
		}else{
			int totalUnits = MZInfo.size() + UZUInfo.size();			
			int infoPerRow = 7;
			vector<vector<int>> tempUnitsInfo;
			vector<vector<int>> UnitsInfo;
			vector<int> tempSingleUnit;
			int unitCounter = -1;

			int sizeOfMZInfo = MZInfo.size();
			for(int tempr = 0; tempr<sizeOfMZInfo ;tempr++){
				unitCounter++;
				for(int tempc = 0; tempc<infoPerRow ;tempc++){
					tempSingleUnit.push_back(MZInfo[tempr][tempc]);
				}
				tempUnitsInfo.push_back(tempSingleUnit);
				tempSingleUnit.clear();
			}

			int sizeOfUZInfo = UZUInfo.size();
			for(int tempr = 0; tempr<sizeOfUZInfo ;tempr++){
				unitCounter++;
				for(int tempc = 0; tempc<infoPerRow ;tempc++){
					tempSingleUnit.push_back(UZUInfo[tempr][tempc]);
				}
				tempUnitsInfo.push_back(tempSingleUnit);
				tempSingleUnit.clear();
			}
			tempSingleUnit.~vector();

			UnitsInfo = sortByRows(tempUnitsInfo, infoPerRow, 2);
			tempUnitsInfo.~vector();

			startIndex = 0;
			endIndex = count + tempUZUCounter + 1; 

			for(int wordCopyIndx = startIndex;wordCopyIndx<=endIndex;wordCopyIndx++){
				UnitInformation uif;

				uif.lineNumber = lineNumber;
				uif.wordNumber = wordNumber;
				uif.unitNumber = wordCopyIndx;
				uif.top = UnitsInfo[wordCopyIndx][0];
				uif.bottom = UnitsInfo[wordCopyIndx][1];
				uif.left = UnitsInfo[wordCopyIndx][2];
				uif.right = UnitsInfo[wordCopyIndx][3];
				uif.height = UnitsInfo[wordCopyIndx][4];
				uif.width = UnitsInfo[wordCopyIndx][5];
				uif.zoneNumber = UnitsInfo[wordCopyIndx][6];
				SEFLineUnitsInfo.push_back(uif);
			}

			UnitsInfo.~vector();
		}

		// free memory
		MZUInfo.~vector();
		UZUInfo.~vector();
		MZInfo.~vector();
	} 
	
	return SEFLineUnitsInfo;
}


vector<vector<int> > HSegmenter::getUnitsFromTouchingPoints(vector<int> TPoints, int** SI, int numR, int numC){
	vector<vector<int> > Units;
	int wd =  numC;

	int thValPointDiff = 3;
	int numOfPoints = TPoints.size();
	vector<int> Points;

	if(TPoints[0] >= thValPointDiff){
		Points.push_back(0);
	}else{
		TPoints.at(0) = 0;
	}

	for(int numOfPointsCounter = 0; numOfPointsCounter < numOfPoints-1; numOfPointsCounter++){
		if(TPoints[numOfPointsCounter+1] - TPoints[numOfPointsCounter] > thValPointDiff){
			Points.push_back(TPoints[numOfPointsCounter]);
		}
	}

	if(wd - TPoints[numOfPoints-1] > thValPointDiff){
		Points.push_back(TPoints[numOfPoints-1]);
		Points.push_back(wd-1);
	}else{
		Points.push_back(wd-1);
	}

	numOfPoints = Points.size();

	if(numOfPoints < 2){
		return Units;
	}

	int countPoint = -1;
	vector<int> AcPoints;

	countPoint++;
	AcPoints.push_back(Points[0]);

	int tempSIR;
	int tempSIC;
	int** tempSI;
	bool validity;

	tempSI = cropingImage(SI, 0, numR-1, Points[0], Points[1]); //////////
	tempSIR = (numR-1) - 0 + 1;
	tempSIC = Points[1] - Points[0] + 1;

	validity = checkValidityUsingBottom(tempSI, tempSIR, tempSIC);

	if(validity){
		countPoint++;
		AcPoints.push_back(Points[countPoint]);
	}

	int startPoint , endPoint;
	for(int numOfPointsCounter=1 ; numOfPointsCounter<numOfPoints-1; numOfPointsCounter++){
		startPoint = Points[numOfPointsCounter]+1;
		endPoint = Points[numOfPointsCounter+1];

		tempSI = cropingImage(SI, 0, numR-1, startPoint, endPoint);
		tempSIR = (numR-1) - 0 + 1;
		tempSIC = endPoint - startPoint + 1;

		validity = checkValidityUsingBottom(tempSI, tempSIR, tempSIC);

		if(validity){
			countPoint++;
			AcPoints.push_back(endPoint);
		}else{
			AcPoints.push_back(endPoint);
		}
	}

	delete2DIntArray(tempSI, tempSIR);

	vector<int> tempUnit;

	tempUnit.push_back(AcPoints[0]);
	tempUnit.push_back(AcPoints[1]-1);
	Units.push_back(tempUnit);
	tempUnit.clear();

	if(countPoint > 2){
		for(int numOfPointsCounter=1 ; numOfPointsCounter<countPoint-2; numOfPointsCounter++){
			tempUnit.push_back(AcPoints[numOfPointsCounter]+1);
			tempUnit.push_back(AcPoints[numOfPointsCounter+1]-1);
			Units.push_back(tempUnit);
			tempUnit.clear();
		}

		tempUnit.push_back(AcPoints[countPoint-1]+1);
		tempUnit.push_back(AcPoints[countPoint]);
		Units.push_back(tempUnit);
		tempUnit.clear();
	}

	// free memory
	Points.~vector();
	AcPoints.~vector();

	return Units;
}
vector<int> HSegmenter::eliminateTouchingError(int** I, int numR, int numC){
	int** TI = allocate2DIntArray(numR, numC);
	vector<int> TPoints;
	

	int thWd = 2;
	int thHt = 2; 

	int ht = numR;
	int wd = numC;

	for(int ti=0; ti<numR; ti++)
		for(int tj=0; tj<numC; tj++)
			TI[ti][tj] = I[ti][tj];


	for(int hti=thHt; hti<ht-1; hti++){
		for(int wdi=thWd; wdi<wd-thWd; wdi++){
			TI[hti][wdi] = (I[hti][wdi] | I[hti+1][wdi]);
		}
	}

	for(int wdi=0; wdi<wd; wdi++){
		TI[ht-1][wdi] = 1;	
	}

	for(int wdi = thWd+1; wdi<wd-thWd+1; wdi++){
		if((ht - getSum('c', wdi, TI, numR, numC)) == 0){
			TPoints.push_back(wdi);
		}
	}

	if(TPoints.empty()){
		for(int ti=0; ti<numR; ti++)
			for(int tj=0; tj<numC; tj++)
				TI[ti][tj] = I[ti][tj];

		for(int wdi=thWd+1; wdi<wd-thWd; wdi++){
			for(int hti = 0;hti<ht-1;hti++){
				TI[hti][wdi] = (I[hti][wdi] | I[hti][wdi+1]);
			}
		}

		for(int wdi = thWd+1;wdi<wd-thWd+1;wdi++)
			if((ht - getSum('c', wdi, TI, numR, numC)) == 0){
				TPoints.push_back(wdi);
			}
	}

	if(TPoints.empty()){
		vector<int> CR;
		int tempTIR;
		int tempTIC;
		int** tempTI;

		tempTIR = (ht-1) - 1 + 1;
		tempTIC = (wd-1) - 0 + 1;
		tempTI = cropingImage(TI, 1, ht-1, 0, wd-1);

		CR = getCrossing(tempTI, tempTIR, tempTIC);	

		// free memory
		delete2DIntArray(tempTI, tempTIR);

		vector<int> PI;

		for(int tempCount = 0; tempCount<thWd; tempCount++){
			PI.push_back(-1);
		}

		for(int tempCount = thWd; tempCount<wd-thWd+1; tempCount++){
			PI.push_back(ht - getSum('c', tempCount, TI, numR, numC));
		}

		for(int tempCount = thWd+1; tempCount<wd-thWd+1; tempCount++){
			if(CR[tempCount] == 1 && PI[tempCount]==1 && (float)(wd-tempCount)/ht > 0.45){
				if((float)(tempCount-1)/ht < 0.45){
					tempTIR = (numR-1) - 0 + 1;
					tempTIC = (tempCount-1) - 0 + 1;
					tempTI = cropingImage(TI, 0, numR-1, 0, tempCount-1);

					int vBarLoc = getVerticalBar(tempTI, tempTIR, tempTIC);

					// free memory
					delete2DIntArray(tempTI, tempTIR);

					if(vBarLoc > 0){
						TPoints.push_back(tempCount);
					}
					else{
						TPoints.push_back(tempCount);
					}
				}else if(CR[tempCount] == 1 && PI[tempCount]==2 && ( (PI[tempCount-1] >= (float)ht* 0.7) | (PI[tempCount-2] >= (float)ht* 0.7) ) && (wd-tempCount)/(float)ht > 0.4){
					int crLoc = 1;
					for(int crLocCount = 0; crLocCount<ht; crLocCount++){
						if(TI[crLocCount][tempCount]==0){
							crLoc = crLocCount;
							break;
						}
					}

					if((float)crLoc/ht < 0.25){
						int wdOfPrevUnit;
						if(TPoints.empty()){
							wdOfPrevUnit = tempCount - 1;
						}else{
							wdOfPrevUnit = tempCount - TPoints[TPoints.size() -1];
						}

						float wdVshtRatio = (float)wdOfPrevUnit / ht;

						if(wdVshtRatio < 0.4){
							TPoints.push_back(tempCount);
						}
					}
					else{
						TPoints.push_back(tempCount);
					}

				} 
			} 
		}

		CR.~vector();
		PI.~vector();
	}

	delete2DIntArray(TI, numR);

	return TPoints;
}

vector<int> HSegmenter::rlhist(int** I, int r, int c){
	vector<int> RLHist;

	int **J = allocate2DIntArray(r, c);

	for(int i=0;i<r;i++){
		for(int j=0;j<c;j++){
			if(!I[i][j])
				J[i][j]=1;
			else
				J[i][j]=0;
		}
	}

	int **K = allocate2DIntArray(r, c);	

	for(int i=0;i<r;i++){
		for(int j=0;j<c;j++){
			K[i][j]=0;
		}
	}

	for(int i=0;i<r;i++)
		K[i][0]=J[i][0];

	int rlhistVal;
	for(int i = 0;i<r;i++){
		for(int j = 1 ;j<c;j++){
			if (J[i][j] == 1)
				K[i][j] = K[i][j-1] + J[i][j];
			else
				K[i][j] = 0;
		}

		rlhistVal = 0;
		for(int j = 0 ;j<c;j++){
			rlhistVal += K[i][j];
		}
		RLHist.push_back(rlhistVal);
	}

	delete2DIntArray(J, r);

	delete2DIntArray(K, r);

	return RLHist;
}

int** HSegmenter::allocate2DIntArray(int r, int c)
{
	int** arraytmp;
	arraytmp = new int *[r];
	for(int i=0;i<r;i++)
		arraytmp[i] = new int [c];

	return arraytmp;
}

bool** HSegmenter::allocate2DBoolArray(int r, int c)
{
	bool** arraytmp;
	arraytmp = new bool *[r];
	for(int i=0;i<r;i++)
		arraytmp[i] = new bool [c];

	return arraytmp;
}

void HSegmenter::delete2DIntArray(int** arraytmp, int r){
	for(int i=0; i<r; i++)
		delete[] arraytmp[i];

	delete[] arraytmp;
}
void HSegmenter::delete2DBoolArray(bool** arraytmp, int r){
	for(int i=0; i<r; i++)
		delete[] arraytmp[i];

	delete[] arraytmp;
}
float HSegmenter::getMean(int* a, int N){
	float sum=0;
	for(int i=0;i<N;i++){
		sum=sum+a[i];
	}

	return sum/N;
}

float HSegmenter::getMean(vector<int> a){
	float sum=0;
	int N = a.size();
	for(int i=0;i<N;i++){
		sum=sum+a[i];
	}

	return sum/N;
}

float HSegmenter::getMedian(int* a, int N){
	int* temp = new int[N];
	for(int  i= 0;i<N;i++){
		temp[i] = a[i];
	}
	sort(temp, temp + N);

	float medVal = (float)temp[(int)(ceil((double)(N/2)))];
	delete[] temp;

	return medVal;
}

float HSegmenter::getMedian(vector<int> a){
	int N = a.size();
	int* temp = new int[N];
	for(int  i= 0;i<N;i++){
		temp[i] = a[i];
	}
	sort(temp, temp + N);

	float medVal = (float)temp[(int)(ceil((double)(N/2)))];
	delete[] temp;

	return medVal;
}

float HSegmenter::getMedian2(vector<int> a){
	int N = a.size();

	if(N==1){
		return a[0];
	}

	int* temp = new int[N];
	for(int  i= 0;i<N;i++){
		temp[i] = a[i];
	}
	sort(temp, temp + N);

	float medVal = 0;

	if(N%2==0){
		medVal = (float) (temp[(int)(N/2)-1] + temp[(int)(N/2)])/2;
	}else{
		medVal = (float) temp[(int)(N/2)-1];
	}

	delete[] temp;

	return medVal;
}

float HSegmenter::getStdDev(int* a, int N){
	if(N>1){
		float var=0;
		float meanVal = getMean(a, N);	
		for (int i=0; i<N; i++){
			var += (a[i] - meanVal) * (a[i] - meanVal);
		}


		return sqrt(var/(N-1));
	}
	else{
		return 0;
	}
}

float HSegmenter::getStdDev(vector<int> a){
	int N = a.size();
	if(N>1){
		float var=0;
		float meanVal = getMean(a);	
		for (int i=0; i<N; i++){
			var += (a[i] - meanVal) * (a[i] - meanVal);
		}


		return sqrt(var/(N-1));
	}
	else{
		return 0;
	}
}

int HSegmenter::getMaxVal(int* a, int limit){
	int maxVal = a[0];
	for(int i=1;i<limit;i++){
		if(a[i]>maxVal){
			maxVal=a[i];
		}
	}
	return maxVal;
}

int HSegmenter::getMaxVal(vector<int> a){
	int limit = a.size();
	int maxVal = a[0];
	for(int i=1;i<limit;i++){
		if(a[i]>maxVal){
			maxVal=a[i];
		}
	}
	return maxVal;
}

int HSegmenter::getMaxIndx(int* a, int limit){
	int maxVal=a[0];
	int indx=0;
	for(int i=1;i<limit;i++){
		if(a[i]>maxVal){
			maxVal=a[i];
			indx=i;
		}
	}
	return indx;
}
int HSegmenter::getMaxIndx(vector<int> a){
	int limit = a.size();
	int maxVal=a[0];
	int indx=0;
	for(int i=1;i<limit;i++){
		if(a[i]>maxVal){
			maxVal=a[i];
			indx=i;
		}
	}
	return indx;
}
int HSegmenter::getMaxIndxInRange(int* a,int start, int end){
	int N = end-start+1; 
	int* aa = new int[N];

	for (int i=start, ii=0 ; i<=end; i++, ii++){
		aa[ii] = a[i];
	}

	int maxVal=aa[0];
	int indx=0;

	for(int i=1;i<N;i++){
		if(aa[i]>maxVal){
			maxVal=aa[i];
			indx=i;
		}
	}
	delete[] aa;
	return indx;
}	

int HSegmenter::getMaxIndxInRange(vector<int> a,int start, int end){
	int N = end-start+1; 
	int* aa = new int[N];

	for (int i=start, ii=0 ; i<=end; i++, ii++){
		aa[ii] = a[i];
	}

	int maxVal=aa[0];
	int indx=0;

	for(int i=1;i<N;i++){
		if(aa[i]>maxVal){
			maxVal=aa[i];
			indx=i;
		}
	}
	delete[] aa;
	return indx;
}	

int HSegmenter::getMinVal(int* a, int limit){
	int minVal=a[0];
	for(int i=1;i<limit;i++){
		if(a[i]<minVal){
			minVal=a[i];
		}
	}
	return minVal;
}

int HSegmenter::getMinVal(vector<int> a){
	int limit = a.size();
	int minVal=a[0];
	for(int i=1;i<limit;i++){
		if(a[i]<minVal){
			minVal=a[i];
		}
	}
	return minVal;
}
int HSegmenter::getMinIndx(int* a, int limit){
	int minVal=a[0];
	int indx=0;
	for(int i=1;i<limit;i++){
		if(a[i]<minVal){
			minVal=a[i];
			indx=i;
		}
	}
	return indx;
}
int HSegmenter::getMinIndx(vector<int> a){
	int limit = a.size();
	int minVal=a[0];
	int indx=0;
	for(int i=1;i<limit;i++){
		if(a[i]<minVal){
			minVal=a[i];
			indx=i;
		}
	}
	return indx;
}
int HSegmenter::getMinIndxInRange(int* a,int start,int end){
	int N = end-start+1; 
	int* aa = new int[N];

	for (int i=start, ii=0 ; i<=end; i++, ii++){
		aa[ii] = a[i];
	}

	int minVal=aa[0];
	int indx=0;

	for(int i=1;i<N;i++){
		if(aa[i]<minVal){
			minVal=aa[i];
			indx=i;
		}
	}

	delete[] aa;

	return indx;
}

int HSegmenter::getMinIndxInRange(vector<int> a,int start,int end){
	int N = end-start+1; 
	int* aa = new int[N];

	for (int i=start, ii=0 ; i<=end; i++, ii++){
		aa[ii] = a[i];
	}

	int minVal=aa[0];
	int indx=0;

	for(int i=1;i<N;i++){
		if(aa[i]<minVal){
			minVal=aa[i];
			indx=i;
		}
	}

	delete[] aa;

	return indx;
}

int HSegmenter::getSum(char flag, int num, int** I, int r, int c){
	int sum=0;
	if(flag=='r'){
		for (int j = 0; j<c; j++)
			sum += I[num][j];
	}
	else{
		for (int i = 0; i<r; i++)
			sum += I[i][num];
	}

	return sum;
}	
int HSegmenter::getSumLR(int** I, int num, int left, int right){
	int sum=0;

	for (int j = left; j<=right; j++)
		sum += I[num][j];

	return sum;
}


int** HSegmenter::cropingImage(int** Simage, int top, int bottom, int left, int right){
	int numrow=(bottom-top+1);
	int numcol=(right-left+1);

	int** corped_image = allocate2DIntArray(numrow,numcol);
	for(int i=top,ni=0;i<=bottom;i++,ni++)
		for(int j=left,nj=0;j<=right;j++,nj++){
			corped_image[ni][nj]=Simage[i][j];
		}

		return corped_image;
}

void HSegmenter::getMatraaLocPH(int** TWord, int r1, int c1, int &ed_index_up_zone_ph, int &st_index_ml_zone_ph)
{
	vector<int> HH;
	for(int i=0;i<r1;i++){
		HH.push_back(c1 - getSum('r', i, TWord, r1, c1));
	}

	int size = r1;
	int inDx = getMaxIndx(HH);

	float matraaThVal = 0.7f * HH[inDx];
	bool stat = true;

	ed_index_up_zone_ph = inDx-1;
	st_index_ml_zone_ph = inDx+1;

	stat = true;
	for(int tmr = inDx-1 ; tmr>=0; tmr--){
		if(stat && (HH[tmr] >= matraaThVal)){
			ed_index_up_zone_ph = tmr-1;
		}else{
			stat = false;
			break;
		}
	}

	stat = true;
	for(int tmr=inDx+1; tmr<=ceil((float)r1/2); tmr++){
		if(stat && (HH[tmr] >= matraaThVal)){
			st_index_ml_zone_ph = tmr+1;
		}else{
			stat = false;
			break;
		}
	}

	HH.~vector();
}



vector<point> HSegmenter::find(int color, int** I, int r, int c){
	point p;
	vector<point> points;
	for(int i=0;i<r;i++){
		for(int j=0;j<c;j++){
			if(I[i][j]==color){
				p.x=i;
				p.y=j;
				points.push_back(p);
			}
		}
	}
	return points;
}
vector<vector<int>> HSegmenter::sortByRows(vector<vector<int>> arrayToSort, int numOfElement, int index){
	int arrSize = arrayToSort.size();

	vector<vector<int>> sorted_units_info;
	vector<int> singleUnit;

	vector<vector<int> > tempArray(arrSize, vector<int>(2));

	for(int i=0; i<arrSize; i++){
		tempArray[i][0] =  i ;
		tempArray[i][1] =  arrayToSort[i][index];
	}	

	int tempIndex;
	int tempValue;

	for(int i = arrSize-1; i>=0; i--){
		for(int j = 0; j<i; j++){
			if(tempArray[j][1] > tempArray[j+1][1]){
				tempIndex = tempArray[j][0];
				tempValue = tempArray[j][1];

				tempArray[j][0] = tempArray[j+1][0];
				tempArray[j][1] = tempArray[j+1][1];

				tempArray[j+1][0] =  tempIndex;
				tempArray[j+1][1] = tempValue;
			}
		}
	}

	for( int i=0; i<arrSize; i++){
		index = tempArray[i][0];
		for( int j = 0; j<numOfElement; j++){
			singleUnit.push_back(arrayToSort[index][j]);
		}
		sorted_units_info.push_back(singleUnit);
		singleUnit.clear();
	}
	singleUnit.~vector();
	tempArray.~vector();

	return sorted_units_info;
}
vector<int> HSegmenter::copyDesiredColumn(vector<vector<int>> srcArray, int columnToCopy){
	vector<int> destArray;
	int totalRow = srcArray.size();

	for(int i=0; i<totalRow; i++){
		destArray.push_back(srcArray[i][columnToCopy]);
	}

	return destArray;
}
bool HSegmenter::checkValidityUsingBottom(int** I, int numR, int numC){
	bool validity;

	int r = numR;
	int c = numC;
	int loc = 0;

	for(int i=r-1; i>-1 ; i--){
		if((c - getSum('r', i, I, r, c)) != 0){
			loc = i;
			break;
		}
	}

	if(loc > 2*(float)r/3){
		validity = true;
	}else{
		validity = false;
	}

	return validity;
}

vector<int> HSegmenter::getCrossing(int** I, int numR, int numC){
	vector<int> CR;

	int rNum = numR;
	int cNum = numC;

	for(int col = 0;col<cNum;col++){
		int crCount = 0;

		if(I[0][col] == 0){
			crCount++;
		}

		for(int row = 0; row<rNum-1; row++){
			if(I[row][col]==1 && I[row+1][col]==0){
				crCount++;
			}
		}
		CR.push_back(crCount);
	}

	return CR;
}

int HSegmenter::getVerticalBar(int** I, int numR, int numC){
	int vBarLoc = -1;

	int r=numR;
	int c=numC;

	int* VH = new int[c];
	for(int i = 0;i<c;i++){
		VH[i] = r - getSum('c', i, I, numR, numC);
	}

	int val,loc;
	val= getMaxVal(VH, c);
	loc= getMaxIndx(VH, c);

	if(val >= 0.8 * (float)r){
		vBarLoc = loc;
	}

	delete[] VH;

	return vBarLoc;
}
