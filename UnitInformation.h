///////////////////////////////////////////////////////////////////////
// File:        UnitInformation.h
// Description: Container of segmented units information.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////


#pragma once

class UnitInformation
{
public:
	UnitInformation(void);

	int top;			
	int bottom;			
	int left;			
	int right;			
	int height;			
	int width;			
	int matraaSrartLoc;	
	int matraaEndLoc;	
	int lineNumber;		
	int wordNumber;		
	int unitNumber;		
	int subUnitNumber;	
	int zoneNumber;		
	int locOfShadowUnit;
	int colorNumber;	
};
