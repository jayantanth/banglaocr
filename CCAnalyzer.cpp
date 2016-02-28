///////////////////////////////////////////////////////////////////////
// File:        CCAnalyzer.cpp
// Description: Connected component analyzer.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CCAnalyzer.h"

CCAnalyzer::CCAnalyzer(bool **b, int x, int y, bool advMood)
	{	
		this->BArray = b;

		this->xsize = x;
		this->ysize = y;		

		// labels = structure with dimensions of data, initialized with the value of Background
		this->LabelArray = new int * [this->xsize];
		for (int i = 0; i < this->xsize; i++)
		{
			this->LabelArray[i] = new int [this->ysize];
			for (int j = 0; j < this->ysize; j++)
			{
				LabelArray[i][j] = 0;
			}
		}

		Parent = gcnew System::Collections::ArrayList();
		Rank = gcnew System::Collections::ArrayList();
		
		advanceMood = advMood;

		if(!advanceMood){
			this->labelCC();
		}
	}// end 

CCAnalyzer::CCAnalyzer(int **b, int x, int y, bool advMood)
	{
		this->xsize = x;
		this->ysize = y;		

		// labels = structure with dimensions of data, initialized with the value of Background
		this->LabelArray = new int * [this->xsize];
		this->BArray = new bool * [this->xsize];
		for (int i = 0; i < this->xsize; i++)
		{
			this->LabelArray[i] = new int [this->ysize];
			this->BArray[i] = new bool [this->ysize];
			for (int j = 0; j < this->ysize; j++)
			{
				if(b[i][j]){
					BArray[i][j] = true;
				}else{
					BArray[i][j] = false;
				}

				LabelArray[i][j] = 0;
			}
		}

		Parent = gcnew System::Collections::ArrayList();
		Rank = gcnew System::Collections::ArrayList();

		advanceMood = advMood;

		if(!advanceMood){
			this->labelCC();
		}
	}// end 

void CCAnalyzer::labelCC()
{
	// Declaration
	point p;
	vector<point> neighbours;
	vector<int> L;

	// linked = []
	vector<int> linked;

	int nextLabel = 1;
    int currLabel;

	Rank->Add(0);	
	Parent->Add(-1); // dummy

	// First pass //
   
	//for column in row:
	for(int col=0; col<this->ysize; col++){
		// for row in data:
		for(int row=0; row<this->xsize; row++){
			// if data[row][col] is not Background
			if(!BArray[row][col]){
               // neighbours = connected elements with the current element's label
               // search the eight neighbors
				neighbours.clear();
				L.clear();

				if(row-1>=0 && !BArray[row-1][col]){
					p.x = row-1;
					p.y = col;
					neighbours.push_back(p);
					L.push_back(LabelArray[row-1][col]);
				}
				if(row-1>=0 && col-1>=0 && !BArray[row-1][col-1]){
					p.x = row-1;
					p.y = col-1;
					neighbours.push_back(p);
					L.push_back(LabelArray[row-1][col-1]);
				}
				if(col-1>=0 && !BArray[row][col-1]){
					p.x = row;
					p.y = col-1;
					neighbours.push_back(p);
					L.push_back(LabelArray[row][col-1]);
				}		
				if(row+1<xsize && col-1>=0 &&!BArray[row+1][col-1]){
					p.x = row+1;
					p.y = col-1;
					neighbours.push_back(p);
					L.push_back(LabelArray[row+1][col-1]);
				}

                //if neighbours is empty
				if(neighbours.size() == 0){
					//linked[NextLabel] = set containing NextLabel
					linked.push_back(nextLabel);
					
					// labels[row][column] = nextLabel
					Parent->Add(nextLabel);
					Rank->Add(0);
					LabelArray[row][col] = nextLabel;
					
					// NextLabel += 1
					nextLabel++;               
				}else{                   
                   // Find the smallest label
                   // L = neighbours labels - Already containing the labels
                   LabelArray[row][col] = getMinVal(L);
                   currLabel = LabelArray[row][col];

				   /*for label in L
                       linked[label] = union(linked[label], L)*/
 		
				   for(int labelNum=0; labelNum < L.size(); labelNum++){
					   if(currLabel != L[labelNum]){
							Union(currLabel, L[labelNum]);
					   }
				   }

				} // if(neighbours.size() == 0){
			} // if(BArray[row][col] != 0){
		} // for(int row=1;
	} // for(int col=1; 

	// End of First pass //

	// Second pass

	//for column in row:
	for(int col=0; col<this->ysize; col++){
		// for row in data:
		for(int row=0; row<this->xsize; row++){
			// if labels[row][column] is not Background
			if(LabelArray[row][col] != 0){
				// labels[row][col] = Find(labels[row][col])     
				LabelArray[row][col] = Find(LabelArray[row][col]);	
			}
		}
	}

	// End of Second pass //
	numberOflabels = renumber_labels(1) - 1;

	/*
	System::IO::StreamWriter^ sr = gcnew System::IO::StreamWriter("C:\\renum.txt");
	int currLab;
	for (int i = 0; i < xsize; i++)
	{
		for (int j = 0; j < ysize; j++)
		{
			currLab = LabelArray[i][j];
			if (currLab>0)
				sr->Write(" "+currLab);
			else
				sr->Write(" "+0);
		}
		sr->WriteLine();
	}
	sr->Close();
	*/
}

int CCAnalyzer::renumber_labels(int start) {
        map<int,int> translation;
        int n = start;
		
		for (int i = 0; i < xsize; i++)
		{
			for (int j = 0; j < ysize; j++)
			{
				int pixel = LabelArray[i][j];
				if(pixel==0) continue;
				map<int,int>::iterator p = translation.find(pixel);
				if(p==translation.end())
					translation.insert(pair<int,int>(pixel,n++));
			}
		}

        n = start;
        for(map<int,int>::iterator p = translation.begin();p!=translation.end();p++) {
            p->second = n++;
        }

		vector<vector<point>> CCS(n-1);

		for (int i = 0; i < xsize; i++)
		{
			for (int j = 0; j < ysize; j++)
			{
				int pixel = LabelArray[i][j];
				if(pixel==0) continue;
				LabelArray[i][j] = translation[pixel];
				CCS[translation[pixel]-1].push_back(point(i, j));
			}
		}
        return n;
    }

vector<vector<point>> CCAnalyzer::labelCC_adv_mood()
{
	// Declaration
	point p;
	vector<point> neighbours;
	vector<int> L;

	// linked = []
	vector<int> linked;

	int nextLabel = 1;
    int currLabel;

	Rank->Add(0);	
	Parent->Add(-1); // dummy

	// First pass //
   
	//for column in row:
	for(int col=0; col<this->ysize; col++){
		// for row in data:
		for(int row=0; row<this->xsize; row++){
			// if data[row][col] is not Background
			if(!BArray[row][col]){
               // neighbours = connected elements with the current element's label
               // search the eight neighbors
				neighbours.clear();
				L.clear();

				if(row-1>=0 && !BArray[row-1][col]){
					p.x = row-1;
					p.y = col;
					neighbours.push_back(p);
					L.push_back(LabelArray[row-1][col]);
				}
				if(row-1>=0 && col-1>=0 && !BArray[row-1][col-1]){
					p.x = row-1;
					p.y = col-1;
					neighbours.push_back(p);
					L.push_back(LabelArray[row-1][col-1]);
				}
				if(col-1>=0 && !BArray[row][col-1]){
					p.x = row;
					p.y = col-1;
					neighbours.push_back(p);
					L.push_back(LabelArray[row][col-1]);
				}		
				if(row+1<xsize && col-1>=0 &&!BArray[row+1][col-1]){
					p.x = row+1;
					p.y = col-1;
					neighbours.push_back(p);
					L.push_back(LabelArray[row+1][col-1]);
				}

                //if neighbours is empty
				if(neighbours.size() == 0){
					//linked[NextLabel] = set containing NextLabel
					linked.push_back(nextLabel);
					
					// labels[row][column] = nextLabel
					Parent->Add(nextLabel);
					Rank->Add(0);
					LabelArray[row][col] = nextLabel;
					
					// NextLabel += 1
					nextLabel++;               
				}else{                   
                   // Find the smallest label
                   // L = neighbours labels - Already containing the labels
                   LabelArray[row][col] = getMinVal(L);
                   currLabel = LabelArray[row][col];

				   /*for label in L
                       linked[label] = union(linked[label], L)*/
 		
				   for(int labelNum=0; labelNum < L.size(); labelNum++){
					   if(currLabel != L[labelNum]){
							Union(currLabel, L[labelNum]);
					   }
				   }

				} // if(neighbours.size() == 0){
			} // if(BArray[row][col] != 0){
		} // for(int row=1;
	} // for(int col=1; 

	// End of First pass //

	// Second pass

	//for column in row:
	for(int col=0; col<this->ysize; col++){
		// for row in data:
		for(int row=0; row<this->xsize; row++){
			// if labels[row][column] is not Background
			if(LabelArray[row][col] != 0){
				// labels[row][col] = Find(labels[row][col])     
				LabelArray[row][col] = Find(LabelArray[row][col]);	
			}
		}
	}

	// End of Second pass //
	vector<vector<point>> CC = renumber_labels_adv_mood(1);
	numberOflabels = CC.size();	
	/*
	System::IO::StreamWriter^ sr = gcnew System::IO::StreamWriter("C:\\renum.txt");
	int currLab;
	for (int i = 0; i < xsize; i++)
	{
		for (int j = 0; j < ysize; j++)
		{
			currLab = LabelArray[i][j];
			if (currLab>0)
				sr->Write(" "+currLab);
			else
				sr->Write(" "+0);
		}
		sr->WriteLine();
	}
	sr->Close();
	*/

	return CC;
}

vector<vector<point>> CCAnalyzer::renumber_labels_adv_mood(int start) {
        map<int,int> translation;
        int n = start;
		
		for (int i = 0; i < xsize; i++)
		{
			for (int j = 0; j < ysize; j++)
			{
				int pixel = LabelArray[i][j];
				if(pixel==0) continue;
				map<int,int>::iterator p = translation.find(pixel);
				if(p==translation.end())
					translation.insert(pair<int,int>(pixel,n++));
			}
		}

        n = start;
        for(map<int,int>::iterator p = translation.begin();p!=translation.end();p++) {
            p->second = n++;
        }

		vector<vector<point>> CCS(n-1);

		for (int i = 0; i < xsize; i++)
		{
			for (int j = 0; j < ysize; j++)
			{
				int pixel = LabelArray[i][j];
				if(pixel==0) continue;
				LabelArray[i][j] = translation[pixel];
				CCS[translation[pixel]-1].push_back(point(i, j));
			}
		}
        return CCS;
    }


void CCAnalyzer::Union(int x, int y){
	int xRoot;
	int yRoot;

	// xRoot := Find(x)
	xRoot = Find(x);

	// yRoot := Find(y)
	yRoot = Find(y);

	/*// xRoot.parent := yRoot
	Parent[xRoot] = yRoot;*/

	// if xRoot.rank > yRoot.rank
	if((int)Rank[xRoot] > (int)Rank[yRoot]){
		// yRoot.parent := xRoot
		Parent[yRoot] = xRoot;
		// else if xRoot.rank < yRoot.rank
	}else if((int)Rank[xRoot] < (int)Rank[yRoot]){
		// xRoot.parent := yRoot
		Parent[xRoot] = yRoot;
		// else if xRoot != yRoot
	}else if(xRoot != yRoot){
		// yRoot.parent := xRoot
		Parent[yRoot] = xRoot;
		// xRoot.rank := xRoot.rank + 1
		Rank[xRoot] = (int)Rank[xRoot] + 1;
	}

}

int CCAnalyzer::Find(int x){
	if((int)Parent[x] == x){
        return x;
	}else{
        Parent[x] = Find((int)Parent[x]);
        return (int)Parent[x];
	}
}

void CCAnalyzer::makeSet(int num){
	
}

int CCAnalyzer::getMinVal(vector<int> a){
	int limit = a.size();
	int minVal=a[0];
	for(int i=1;i<limit;i++){
		if(a[i]<minVal){
			minVal=a[i];
		}
	}
	return minVal;
}

int CCAnalyzer::getNumberOfCC(){
	return numberOflabels;
}
int** CCAnalyzer::getLabeledArray(){
	return LabelArray;	
}
vector<point> CCAnalyzer::getCCPoints(int label){
	point p;
	vector<point> points;
	for(int i=0;i<xsize;i++){
		for(int j=0;j<ysize;j++){
			if(LabelArray[i][j]==label){
				p.x=i;
				p.y=j;
				points.push_back(p);
			}
		}
	}
	return points;
}