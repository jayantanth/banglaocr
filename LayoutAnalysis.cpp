///////////////////////////////////////////////////////////////////////
// File:        LayoutAnalysis.cpp
// Description: Page Layout Analyzer.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "LayoutAnalysis.h"

LayoutAnalysis::LayoutAnalysis(bool **b, int x, int y)
{	
	this->plImage = gcnew System::Drawing::Bitmap(y, x);

	this->R = x;
	this->C = y;	

	// initialize noise removal array
	this->imgArray = new bool * [this->R];
	for (int i = 0; i < this->R; i++)
	{
		this->imgArray[i] = new bool [this->C];
		for (int j = 0; j < this->C; j++)
		{
			if(b[i][j]){
				imgArray[i][j] = 1;
				this->plImage->SetPixel(j, i, System::Drawing::Color::White);
			}else{
				imgArray[i][j] = 0;
				this->plImage->SetPixel(j, i, System::Drawing::Color::Black);
			}

		}
	}
}// end

LayoutAnalysis::LayoutAnalysis(int **b, int x, int y)
{	
	this->plImage = gcnew System::Drawing::Bitmap(y, x);

	this->R = x;
	this->C = y;		

	// initialize noise removal array
	this->imgArray = new bool * [this->R];
	for (int i = 0; i < this->R; i++)
	{
		this->imgArray[i] = new bool [this->C];
		for (int j = 0; j < this->C; j++)
		{
			if(b[i][j]==1){
				imgArray[i][j] = 1;
				this->plImage->SetPixel(j, i, System::Drawing::Color::White);
			}else{
				imgArray[i][j] = 0;
				this->plImage->SetPixel(j, i, System::Drawing::Color::Black);
			}

		}
	}
}// end 

vector<TextBlock> LayoutAnalysis::AnalysisLayout(){
	orderLayout = gcnew System::Collections::ArrayList();
	int val;

	int thresholdValue;
	int upB;
	int downB;
	int leftB;
	int rightB;
	vector<int> boundary;
	vector<TextBlock> textBl;

	// get the text boundary of the text document
	thresholdValue = 20;

    //[I Vb Hb boundary] = getTextBoundary(thresholdValue);
	boundary = getTextBoundary(thresholdValue);

	upB = boundary[0];
	downB = boundary[1];
	leftB = boundary[2];
	rightB = boundary[3];
	
	// Queue tracker
	int	f = 0;  // front of queue
	int	b = 0;  // back of queue

		
	
	// add item (Enque) into the Queue
	TextBlock tb;
	tb.up = upB;
	tb.down  = downB;
	tb.left  = leftB;
	tb.right = rightB;
	tb.split = 'N';
	textBl.push_back(tb);
	val = textBl.size()-1;
	orderLayout->Insert(b, val);

	b++;

	int up_;
	int down_;
	int left_;
	int right_;
	int prevB;
	int countGap;

	int Avg_Hgap_Ht;
	int Std_Hgap_Ht;
	int Avg_Lht;
	int Std_Lht;
	int gapHtTh;
	int stdTh;
	int numOfLine;
	int stLnIndx;
	int topBoundary;
	int downBoundary;

	int internalBlockCount;
	int insetIndex;


	bool isSplit;
	bool split;

	vector<int> Hb;
	vector<int> Vb;
	vector<int> Margine;
	vector<int> Gap_index;
	vector<int> Gap_zlen;
	vector<vector<int>> GapInfo;
	vector<int> Line_index;
	vector<int> Line_zlen;
	vector<vector<int>> LineInfo;

	while(f != b){
		internalBlockCount = 1;

		/// Check Horizontally
		up_ = textBl[f].up;
		down_ = textBl[f].down;
		left_ = textBl[f].left;
		right_ = textBl[f].right;

		// Pick Item (Deque) from the queue
		f = f+1;
		prevB = b;

		// Get the projection
		Hb = getShortProjection(left_, right_, 'H');


		// get the up margine and down margine
		//[up_ down_] = getMargine(Hb,up_,down_);
		Margine = getMargine(Hb, up_, down_);
		up_ = Margine[0];
		down_ = Margine[1];

		// correct the block boundary
		textBl[f-1].up  = up_;
		textBl[f-1].down = down_;

		// calculating number of horizontal gap place, their length
		// [c HGap_index HGap_zlen] = gapSearch(Hb, up_, down_);
		GapInfo = gapSearch(Hb, up_, down_);
		Gap_index = GapInfo[0];
		Gap_zlen = GapInfo[1];

		isSplit = false;    // indicate whether the block has been splitted or not

		countGap = Gap_index.size();


		if(countGap>0){
			/// Statistical Information
			// finding the average horizontal gap height and standard deviation
			Avg_Hgap_Ht = System::Math::Ceiling(getMean(Gap_zlen));    // Avg Horizontal gap Height
			Std_Hgap_Ht = System::Math::Ceiling(getStdDev(Gap_zlen));  // Standard deviation Horizontal gap Height

			// Finding the Horizontal line height
			//[Lht L_index] = blockSearch(HGap_index, HGap_zlen, c, up_, down_);
			LineInfo = blockSearch(GapInfo, up_, down_);
			Line_index = LineInfo[0];
			Line_zlen = LineInfo[1];

			Avg_Lht = System::Math::Ceiling(getMean(Line_zlen));    // Avg Line Height
			Std_Lht = System::Math::Ceiling(getStdDev(Line_zlen));    // Standard deviation of Line Height

			// Determine the paragraph splitter
			// c = 1 means there is only one block so we dont have to do anything
			gapHtTh = Avg_Hgap_Ht + Std_Hgap_Ht;

			if(gapHtTh < 10){
				gapHtTh = 10;
			}

			stdTh = System::Math::Ceiling(Avg_Lht/2);

			split = false;
			numOfLine = 0;
			stLnIndx = 0;

			topBoundary  = up_;
			downBoundary = down_;
			for(int i = 0; i<=countGap-1; i++){
				split = false;

				if(Gap_zlen[i]>=Avg_Lht && Std_Lht<=stdTh){
					// This condition is applicable when the standard deviation of the line height is small
					split = true;
					isSplit = true;
				}else if(Std_Lht>stdTh && Avg_Hgap_Ht<=gapHtTh && Gap_zlen[i]>= gapHtTh){
					// This condition is applicable when the standard deviation of the
					// line height is larger than threshold, the average gap height is
					// less than the gap height threshold and the gap height is greater
					// than the gap height threshold
					split = true;
					isSplit = true;
				}

				if(split){
					// add item (Enque) into the Queue
					tb.up = topBoundary;
					tb.down  = Gap_index[i] - 1;
					tb.left  = left_;
					tb.right = right_;
					tb.split = 'N';
					textBl.push_back(tb);

					insetIndex = orderLayout->IndexOf(f-1)+internalBlockCount;
					val = textBl.size()-1;
					orderLayout->Insert(insetIndex, val);
					internalBlockCount++;

					topBoundary = textBl[b].down + Gap_zlen[i] + 1;
					b++;
				}
			}

			if(isSplit){
				// add item (Enque) into the Queue
				// for the last text block
				tb.up = topBoundary;
				tb.down  = downBoundary;
				tb.left  = left_;
				tb.right = right_;
				tb.split = 'N';
				textBl.push_back(tb);

				insetIndex = orderLayout->IndexOf(f-1)+internalBlockCount;
				val = textBl.size()-1;
				orderLayout->Insert(insetIndex, val);
				internalBlockCount++;

				b++;
			}
		}


		int Avg_Vgap_Wd;
		int Std_Vgap_Wd;
		int Avg_Bwd;
		int Std_Bwd;

		int gapLenTh;

		int leftBoundary;
		int rightBoundary;

		vector<int> Block_index;
		vector<int> Block_zlen;
		vector<vector<int>> BlockInfo;

		if(!isSplit /*& countGap>=2*/){
			// Check Vertically
			// Get the projection
			// Vb = getShortProjection(I(up_:down_, :), 'V');
			Vb = getShortProjection(up_, down_, 'V');

			// get the left margine and right margine
			//[left_ right_] = getMargine(Vb,left_,right_);
			Margine = getMargine(Vb, left_, right_);
			left_ = Margine[0];
			right_ = Margine[1];	

			// correct the block boundary
			textBl[f-1].left  = left_;
			textBl[f-1].right = right_;

			// calculating number of vertical gap place, their length
			// [c VGap_index VGap_zlen] = gapSearch(Vb, left_, right_);
			GapInfo = gapSearch(Vb, left_, right_);
			Gap_index = GapInfo[0];
			Gap_zlen = GapInfo[1];

			countGap = Gap_index.size();

			if(countGap>0){

				/// Statistical Information
				// finding the average vertical gap width and standard deviation
				Avg_Vgap_Wd = System::Math::Ceiling(getMean(Gap_zlen));    // Avg Vertical gap Height
				Std_Vgap_Wd = System::Math::Ceiling(getStdDev(Gap_zlen));    // Standard deviation Vertical gap Height

				// finding the Vertical blocks Width
				// [Bwd B_index] = blockSearch(VGap_index, VGap_zlen, c, left_, right_);
				BlockInfo = blockSearch(GapInfo, left_, right_);
				Block_index = BlockInfo[0];
				Block_zlen = BlockInfo[1];

				Avg_Bwd = System::Math::Ceiling(getMean(Block_zlen));    // Avg Block Width
				Std_Bwd = System::Math::Ceiling(getStdDev(Block_zlen));  // Standard deviation of Block Width

				// Calculate whether to split or not
				// c = 1 means there is only one block so we dont have to do anything
				gapLenTh = 20;
				isSplit = false;

				leftBoundary  = left_;
				rightBoundary = right_;
				for(int i = 0; i<=countGap-1; i++){
					split = false;

					if(Gap_zlen[i]>= gapLenTh && Gap_zlen[i]<=System::Math::Abs(Avg_Bwd - Avg_Vgap_Wd)){
						// This condition implies that the gap will be a splitter of the text
						// block iff it is greater than gapLenTh( Threshold value for the
						// Gap Length) and less than the substraction of the average block
						// width and average gap width
						split = true;
						isSplit = true;
					}else if(Gap_zlen[i]>= (Std_Vgap_Wd + Avg_Vgap_Wd) && Gap_zlen[i]>System::Math::Abs(Avg_Bwd - Avg_Vgap_Wd)){
						// This condition implies that the gap will be a splitter of the text
						// block iff it is greater than the addition of Std_Vgap_Wd &
						// Avg_Vgap_Wd and also greater than the subtraction of average block
						// width and average gap width
						split = true;
						isSplit = true;
					}

					if(split && (Block_index[i] + Block_zlen[i] - leftBoundary + 1)>10){
						tb.up = up_;
						tb.down  = down_;
						tb.left  = leftBoundary;
						tb.right = Block_index[i] + Block_zlen[i];
						tb.split = 'N';
						textBl.push_back(tb);

						insetIndex = orderLayout->IndexOf(f-1)+internalBlockCount;
						val = textBl.size()-1;
						orderLayout->Insert(insetIndex, val);
						internalBlockCount++;

						leftBoundary = textBl[b].right + Gap_zlen[i] + 1;
						b++;
					}else if(split && (Block_index[i] + Block_zlen[i] - leftBoundary + 1)<10){
						leftBoundary = Block_index[i] + Block_zlen[i] + Gap_zlen[i] + 1;
					}
				} // for i = 1 : c-1

				if(isSplit && (rightBoundary - leftBoundary + 1)>10){
					// for the last text block
					tb.up = up_;
					tb.down  = down_;
					tb.left  = leftBoundary;
					tb.right = rightBoundary;
					tb.split = 'N';
					textBl.push_back(tb);

					insetIndex = orderLayout->IndexOf(f-1)+internalBlockCount;
					val = textBl.size()-1;
					orderLayout->Insert(insetIndex, val);
					internalBlockCount++;

					b++;
				}
			} // if(countGap>0)

		}else{ // if(isSplit == 0 & c>=3)
			// Get the projection
			//Vb = getShortProjection(I(up_:down_, :), 'V');
			Vb = getShortProjection(up_, down_, 'V');

			// get the left margine and right margine
			//[left_ right_] = getMargine(Vb,left_,right_);
			Margine = getMargine(Vb, left_, right_);
			left_ = Margine[0];
			right_ = Margine[1];

			// correct the block boundary
			textBl[f-1].left  = left_;
			textBl[f-1].right = right_;
		} // if(isSplit == 0 & c>=3)

		// decided whether the considered block has been splitted or not
		if(prevB != b){
			textBl[f-1].split = 'Y';
		}
	} // while(f > 0)


	/// PageLayout Ordering
	int count = 0;
	vector<TextBlock> Block;

	int inDx;
	if(orderLayout->Count>0){
		for(int i = 0; i<orderLayout->Count; i++){
			inDx = System::Int32::Parse(orderLayout[i]->ToString());
			if(textBl[inDx].split == 'N'){
				count++;
				Block.push_back(textBl[inDx]);
			}
		}
	}

	return Block;
}
void LayoutAnalysis::AnalysisLayoutAndGenerateImage(){
	
	vector<TextBlock> Block = AnalysisLayout();

	///// color the layout image /////
	int numOfBlock = Block.size();
	int lt;
	int rt;
	int tp;
	int bm;

	for(int i=0; i<numOfBlock; i++){
		lt = Block[i].left;	
		rt = Block[i].right;
		tp = Block[i].up;	
		bm = Block[i].down;

		for(int ii=lt; ii<=rt; ii++){
			this->plImage->SetPixel(ii, tp, System::Drawing::Color::Red);
			this->plImage->SetPixel(ii, bm, System::Drawing::Color::Red);
		}

		for(int jj=tp; jj<=bm; jj++){
			this->plImage->SetPixel(lt, jj, System::Drawing::Color::Red);
			this->plImage->SetPixel(rt, jj, System::Drawing::Color::Red);
		}
	}
}

vector<int> LayoutAnalysis::getTextBoundary(int thresholdValue){
	// size of the Image
	int thVal;
	int leftMergine;
	int rightMergine;
	int upMergine;
	int downMergine;

	System::String^ projectionType;

	vector<int> Hb(R);
	vector<int> Vb(C);
	vector<int> Margine;

	thVal = thresholdValue;

	/// Finding the Projections and Profiles from pixel projections
	/// Vertical (Vb) Projection
	projectionType = "V";
	//[I Vb] = getProjection(I, 20, 'V');
	Vb = getProjection(thVal, 'V');


	///// Finding the left and right margine /////
	leftMergine = 0;
	rightMergine = C-1;

	Margine = getMargine(Vb, leftMergine, rightMergine);
	leftMergine = Margine[0];
	rightMergine = Margine[1];

	///// ************************************ /////

	///// calculating number of vertical gap place, their length /////
	// [c VGap_index VGap_zlen] = gapSearch(Vb, l, r);
	vector<int> Gap_index;
	vector<int> Gap_zlen;

	vector<vector<int>> GapInfo = gapSearch(Vb, leftMergine, rightMergine);
	Gap_index = GapInfo[0];
	Gap_zlen = GapInfo[1];
	///// ************************************ /////

	/////////////////////////////////

	int countGap = Gap_zlen.size();
	float Avg_Vgap_Wd;
	float Std_Vgap_Wd;
	float Avg_Bwd;
	float Std_Bwd;
	int len;
	int blockLenTh;
	int part_len;

	vector<vector<int>> BlockInfo;
	vector<int> BlkIndex;
	vector<int> BlkLen;

	if(countGap > 0){
		/// Statistical Information
		// finding the average vertical gap width and standard deviation
		Avg_Vgap_Wd = System::Math::Ceiling(getMean(Gap_zlen));    // Avg Vertical gap Height
		Std_Vgap_Wd = System::Math::Ceiling(getStdDev(Gap_zlen));  // Standard deviation Vertical gap Height


		///// finding the Vertical blocks Width /////
		//[Bwd B_index] = blockSearch(VGap_index, VGap_zlen, c, l, r);
		BlockInfo = blockSearch(GapInfo, leftMergine, rightMergine);
		BlkIndex = BlockInfo[0];
		BlkLen = BlockInfo[1];
		///// ************************************ /////

		Avg_Bwd = System::Math::Ceiling(getMean(BlkLen));    // Avg Block Width
		Std_Bwd = System::Math::Ceiling(getStdDev(BlkLen));     // Standard deviation of Block Width

		///// Eliminating the noisy lines /////
		len = BlkLen.size();

		blockLenTh = 10;        // Block Width Threshold

		part_len = System::Math::Round(C/8);  // Length of the left or right specified boundary
		for(int i = 0; i<len; i++){
			if((BlkLen[i]<=blockLenTh) && (BlkLen[i] <= System::Math::Abs(Avg_Bwd-Std_Bwd))){
				// The condition implies that the Block width must be less than the
				// result of the substraction of Avg_Bwd and Std_Bwd and also the
				// threshold of the block length

				// Vb(B_index(i): B_index(i) + Bwd(i)) = 0;
				// I(:,B_index(i): B_index(i) + Bwd(i))  = 1;

				int blkStIndx = BlkIndex[i];
				int blkEndIndx = BlkIndex[i]+BlkLen[i];

				for(int bi=blkStIndx; bi<=blkEndIndx; bi++){
					Vb[bi] = 0;
					for(int ii=0; ii<R; ii++){	
						imgArray[ii][bi]  = true;
					}
				}
			}
			else if((BlkLen[i] <= blockLenTh*2) && (BlkIndex[i]<=blockLenTh)){
				// For the big noisy object of the left hand side
				// The condition implies that if the Block width is less than twice
				// the 'block width threshold value' and the block starting index
				// must be less than the 'block width threshold value' to ensure
				// that this is not a noisy object due to photocopy

				// Vb(B_index(i): B_index(i) + Bwd(i)) = 0;
				// I(:,B_index(i): B_index(i) + Bwd(i))  = 1;

				int blkStIndx = BlkIndex[i];
				int blkEndIndx = BlkIndex[i]+BlkLen[i];

				for(int bi=blkStIndx; bi<=blkEndIndx; bi++){
					Vb[bi] = 0;
					for(int ii=0; ii<R; ii++){	
						imgArray[ii][bi]  = true;
					}
				}
			}
			else if((BlkLen[i]<=blockLenTh*2) && (BlkIndex[i]+BlkLen[i] >= rightMergine-blockLenTh)){
				// For the big noisy object of the right hand side
				// The condition implies that if the Block width is less than twice
				// the 'block width threshold value' and the block starting index
				// must be less than the 'block width threshold value' to ensure
				// that this is not a noisy object due to photocopy

				int blkStIndx = BlkIndex[i];
				int blkEndIndx = BlkIndex[i]+BlkLen[i];

				for(int bi=blkStIndx; bi<=blkEndIndx; bi++){
					Vb[bi] = 0;
					for(int ii=0; ii<R; ii++){	
						imgArray[ii][bi]  = true;
					}
				}
			}
			else if((BlkLen[i]<=blockLenTh*2) && (BlkLen[i]<=System::Math::Abs(Avg_Bwd-Std_Bwd))){
				// The condition implies that the Block width must be less than the
				// result of the substraction of Avg_Bwd and Std_Bwd and also less 
				// than or equal to the double of threshold value. The additional 
				// conditions are: The text block to delete must reside on to the 
				// 1/8 portion of the total length from left or right side and also
				// the gap of the left/right side must be greater or equal to
				// abs(Avg_Vgap_Wd - Std_Vgap_Wd)
				int Beind = BlkIndex[i] + BlkLen[i]; // Beind : Block End Index

				if(i<len & Beind<part_len && ((BlkIndex[i+1]-Beind)>= System::Math::Abs(Avg_Vgap_Wd - Std_Vgap_Wd))){
					int blkStIndx = BlkIndex[i];
					int blkEndIndx = BlkIndex[i]+BlkLen[i];

					for(int bi=blkStIndx; bi<=blkEndIndx; bi++){
						Vb[bi] = 0;
						for(int ii=0; ii<R; ii++){	
							imgArray[ii][bi]  = true;
						}
					}
				}
				else if(i>1 && BlkIndex[i]>(C - part_len) && (BlkIndex[i]- BlkIndex[i-1] + BlkLen[i-1] >= System::Math::Abs(Avg_Vgap_Wd - Std_Vgap_Wd))){
					int blkStIndx = BlkIndex[i];
					int blkEndIndx = BlkIndex[i]+BlkLen[i];

					for(int bi=blkStIndx; bi<=blkEndIndx; bi++){
						Vb[bi] = 0;
						for(int ii=0; ii<R; ii++){	
							imgArray[ii][bi]  = true;
						}
					}
				}
			}
		}

		///// Re-define the left and right margine /////
		//[l r] = getMargine(Vb, l, r);
		Margine = getMargine(Vb, leftMergine, rightMergine);
		leftMergine = Margine[0];
		rightMergine = Margine[1];
	}
	// free memory
	BlkIndex.~vector();
	BlkLen.~vector();
	BlockInfo.~vector();
	Vb.~vector();
	//////////////////////////////////////

	// Finding the Projections and Profiles from pixel projections
	// Horizontal (Hb) Projection
	// Threshold Value = 20
	//[I Hb] = getProjection(I, 20, 'H');
	Hb = getProjection(thVal, 'H');

	// finding the up and down margine
	//[u d] = getMargine(Hb, u, d);
	///// Finding the left and right margine /////
	upMergine = 0;
	downMergine = R-1;

	Margine = getMargine(Hb, upMergine, downMergine);
	upMergine = Margine[0];
	downMergine = Margine[1];

	///// ************************************ /////

	// calculating number of horizontal gap place, their length
	// [c HGap_index HGap_zlen] = gapSearch(Hb, u, d);
	GapInfo = gapSearch(Hb, upMergine, downMergine);
	Gap_index = GapInfo[0];
	Gap_zlen = GapInfo[1];
	///// ************************************ /////

	countGap = Gap_zlen.size();

	float Avg_Lht;
	float Std_Lht;
	vector<vector<int>> LineInfo;
	vector<int> LineIndex;
	vector<int> LineHt;

	if(countGap > 0){
		/// Statistical Information
		// finding the average horizontal gap height and standard deviation
		Avg_Vgap_Wd = System::Math::Ceiling(getMean(Gap_zlen));    // Avg Horizontal gap Height
		Std_Vgap_Wd = System::Math::Ceiling(getStdDev(Gap_zlen));  // Standard deviation Horizontal gap Height

		// finding the Horizontal line height
		LineInfo = blockSearch(GapInfo, upMergine, downMergine);
		LineIndex = LineInfo[0];
		LineHt = LineInfo[1];

		Avg_Lht = System::Math::Ceiling(getMean(LineHt));    // Avg Line Height
		Std_Lht = System::Math::Ceiling(getStdDev(LineHt));     // Standard deviation of Line Height

		// Eliminating the noisy lines
		len = LineHt.size();

		int LnHtTh = 10;        // Line Height Threshold

		for(int i = 0; i<len; i++){
			if((LineHt[i]<=LnHtTh) && (LineHt[i]<=System::Math::Abs(Avg_Lht-Std_Lht))){
				// Hb(L_index(i): L_index(i) + Lht(i)) = 0;
				// I(L_index(i): L_index(i) + Lht(i),:) = 1;
				int lineStIndx = LineIndex[i];
				int lineEndIndx = LineIndex[i]+LineHt[i];

				for(int li=lineStIndx; li<=lineEndIndx; li++){
					Hb[li] = 0;
					for(int jj=0; jj<C; jj++){	
						imgArray[li][jj]  = true;
					}
				}
			}
		}

		// Re-defined the up and down margine
		//[u d] = getMargine(Hb, u, d);
		Margine = getMargine(Hb, upMergine, downMergine);
		upMergine = Margine[0];
		downMergine = Margine[1];
	}

	// free memory
	LineHt.~vector();
	LineIndex.~vector();
	LineInfo.~vector();
	Hb.~vector();

	GapInfo.~vector();
	Gap_index.~vector();
	Gap_zlen.~vector();
	Margine.~vector();

	// boundary(1) = u;
	// boundary(2) = d;
	// boundary(3) = l;
	// boundary(4) = r;

	vector<int> boundary(4);
	boundary[0] = upMergine;
	boundary[1] = downMergine;
	boundary[2] = leftMergine;
	boundary[3] = rightMergine;

	return boundary;
}

vector<vector<int>> LayoutAnalysis::gapSearch(vector<int> Vb, int lm, int rm){
	vector<vector<int>> gapInfo;
	
	/// Initializing
	int numZ = 0;
	int gapThreshold = 3;
	bool conT = false;
	vector<int> GapIndx;
	vector<int> GapZlen;

	for(int i = lm; i<=rm; i++){
		if(Vb[i]>0 && !conT){
			continue;
		}else if(Vb[i]==0 && conT){
			numZ = numZ + 1;
		}else if(Vb[i]==0 && !conT){
			GapIndx.push_back(i);
			conT = true;
			numZ = numZ + 1;
		}else{
			if(numZ > gapThreshold){
				GapZlen.push_back(numZ);
			}else{
				GapIndx.pop_back();	
			}
			numZ = 0;
			conT = false;
		}
	}

	gapInfo.push_back(GapIndx);
	gapInfo.push_back(GapZlen);

	return gapInfo;
}

vector<vector<int>> LayoutAnalysis::blockSearch(vector<vector<int>> GapInfo, int lm, int rm){
	vector<vector<int>> blockInfo;
	
	vector<int> GapIndex = GapInfo[0];
	vector<int> GapZlen = GapInfo[1];

	int countGap = GapZlen.size();

	// finding the Horizontal line / Vertical blocks Length
	vector<int> BlkIndex;
	vector<int> BlkLen;
	
	BlkIndex.push_back(lm);               // BlkIndex : Line/Block 's starting Index, Ending Index : BlkIndex(i) + BlkLen(i) - 1
	BlkLen.push_back(GapIndex[0] - lm);  // BlkLen : Line/Block 's Length

	for(int i = 1; i<=countGap-1; i++){
		BlkIndex.push_back(GapIndex[i-1] + GapZlen[i-1]);
		BlkLen.push_back(GapIndex[i] - BlkIndex[i]);
	}

	// for getting the Length of the last line/block
	if((rm - (GapIndex[countGap-1] + GapZlen[countGap-1])) > 0){
		BlkIndex.push_back(GapIndex[countGap-1] + GapZlen[countGap-1]);
		BlkLen.push_back(rm - BlkIndex[countGap]);
	}

	blockInfo.push_back(BlkIndex);
	blockInfo.push_back(BlkLen);
	
	BlkIndex.~vector();
	BlkLen.~vector();

	return blockInfo;
}
vector<int> LayoutAnalysis::getProjection(int thVal, char projType){
	vector<int> PRJ;
	int sum;

	if(projType == 'V'){
		// Vertical Projection
		PRJ.resize(C);

		// Get the projection
		for(int j = 0; j<C; j++){
			sum = 0;
			for (int i = 0; i<R; i++){
				if(!imgArray[i][j])
					sum++;
			}
			PRJ[j] = sum;
		}

		// Smoothing the Vertical Projection
		for (int j = 2; j<C-2; j++){
			sum = 0;
			for(int jj = j-2; jj<=j+2; jj++){
				sum += PRJ[jj];
			}

			if(System::Math::Floor(sum/thVal) == 0){
				PRJ[j] = 0;
				// I(i,:) = 1;
				for (int i = 0; i<R; i++){
					imgArray[i][j] = true;
				}
			}
		}

		// for the first two rows
		sum = 0;
		for(int jj = 0; jj<5; jj++){
			sum += PRJ[jj];
		}

		if(System::Math::Floor(sum/thVal) == 0){
			PRJ[0] = 0;
			PRJ[1] = 0;
			for (int i = 0; i<R; i++){
				imgArray[i][0] = true;
				imgArray[i][1] = true;
			}
		}


		// for the last two rows
		sum = 0;
		for(int jj = C-1; jj>=C-5; jj--){
			sum += PRJ[jj];
		}

		if(System::Math::Floor(sum/thVal) == 0){
			PRJ[C-1] = 0;
			PRJ[C-2] = 0;
			for (int i = 0; i<R; i++){
				imgArray[i][C-1] = true;
				imgArray[i][C-2] = true;
			}
		}
	}else{
		// Horizontal Projection
		PRJ.resize(R);

		// Get the projection
		for(int i = 0; i<R; i++){
			sum = 0;
			for (int j = 0; j<C; j++){
				if(!imgArray[i][j])
					sum++;
			}
			PRJ[i] = sum;
		}

		// Smoothing the Horizontal Projection
		for (int i = 2; i<R-2; i++){
			sum = 0;
			for(int ii = i-2; ii<=i+2; ii++){
				sum += PRJ[ii];
			}

			if(System::Math::Floor(sum/thVal) == 0){
				PRJ[i] = 0;
				// I(i,:) = 1;
				for (int j = 0; j<C; j++){
					imgArray[i][j] = true;
				}
			}
		}

		// for the first two rows
		sum = 0;
		for(int ii = 0; ii<5; ii++){
			sum += PRJ[ii];
		}

		if(System::Math::Floor(sum/thVal) == 0){
			PRJ[0] = 0;
			PRJ[1] = 0;
			for (int j = 0; j<C; j++){
				imgArray[0][j] = true;
				imgArray[1][j] = true;
			}
		}


		// for the last two rows
		sum = 0;
		for(int ii = R-1; ii>=R-5; ii--){
			sum += PRJ[ii];
		}

		if(System::Math::Floor(sum/thVal) == 0){
			PRJ[R-1] = 0;
			PRJ[R-2] = 0;
			for (int j = 0; j<C; j++){
				imgArray[R-1][j] = true;
				imgArray[R-2][j] = true;
			}
		}
	}

	return PRJ;
}


vector<int> LayoutAnalysis::getShortProjection(int st, int ed, char projType){
	vector<int> PRJ;
	int sum;
	int thVal = 5;

	if(projType == 'V'){
		// Vertical Projection
		PRJ.resize(C);
		// Get the projection
		for(int j = 0; j<C; j++){
			sum = 0;
			for (int i = st; i<=ed; i++){
				if(!imgArray[i][j])
					sum++;
			}
			PRJ[j] = sum;
		}

		// Smoothing the Vertical Projection
		for (int j = 2; j<C-2; j++){
			sum = 0;
			for(int jj = j-2; jj<=j+2; jj++){
				sum += PRJ[jj];
			}

			if(System::Math::Floor(sum/thVal) == 0){
				PRJ[j] = 0;
			}
		}

		// for the first two rows
		sum = 0;
		for(int jj = 0; jj<5; jj++){
			sum += PRJ[jj];
		}

		if(System::Math::Floor(sum/thVal) == 0){
			PRJ[0] = 0;
			PRJ[1] = 0;
		}


		// for the last two rows
		sum = 0;
		for(int jj = C-1; jj>=C-5; jj--){
			sum += PRJ[jj];
		}

		if(System::Math::Floor(sum/thVal) == 0){
			PRJ[C-1] = 0;
			PRJ[C-2] = 0;
		}
	}else{
		// Horizontal Projection
		PRJ.resize(R);
		// Get the projection
		for(int i = 0; i<R; i++){
			sum = 0;
			for (int j = st; j<=ed; j++){
				if(!imgArray[i][j])
					sum++;
			}
			PRJ[i] = sum;
		}

		// Smoothing the Horizontal Projection
		for (int i = 2; i<R-2; i++){
			sum = 0;
			for(int ii = i-2; ii<=i+2; ii++){
				sum += PRJ[ii];
			}

			if(System::Math::Floor(sum/thVal) == 0){
				PRJ[i] = 0;
			}
		}

		// for the first two rows
		sum = 0;
		for(int ii = 0; ii<5; ii++){
			sum += PRJ[ii];
		}

		if(System::Math::Floor(sum/thVal) == 0){
			PRJ[0] = 0;
			PRJ[1] = 0;
		}


		// for the last two rows
		sum = 0;
		for(int ii = R-1; ii>=R-5; ii--){
			sum += PRJ[ii];
		}

		if(System::Math::Floor(sum/thVal) == 0){
			PRJ[R-1] = 0;
			PRJ[R-2] = 0;
		}
	}

	return PRJ;
}


vector<int> LayoutAnalysis::getMargine(vector<int> proj, int st, int ed){
	vector<int> Margine(2);
	int start = st;
	int end = ed;

	for(int i = start; i<=end; i++){
		if(proj[i]>0){
			start = i;
			break;
		}
	}

	// finding the down margine
	for(int i = end; i>start; i--){
		if(proj[i]>0){
			end = i;
			break;
		}
	}

	Margine[0] = start;
	Margine[1] = end;

	return Margine;
}


float LayoutAnalysis::getMean(vector<int> a){
	float sum=0;
	int N = a.size();
	for(int i=0;i<N;i++){
		sum=sum+a[i];
	}

	return sum/N;
}

float LayoutAnalysis::getStdDev(vector<int> a){
	int N = a.size();
	if(N>1){
		float var=0;
		float meanVal = getMean(a);	
		for (int i=0; i<N; i++){
			var += (a[i] - meanVal) * (a[i] - meanVal);
		}


		return System::Math::Sqrt(var/(N-1));
	}
	else{
		return 0;
	}
}

System::Drawing::Bitmap^ LayoutAnalysis::getLayoutImage(){
	return this->plImage;
}
