///////////////////////////////////////////////////////////////////////
// File:        TextBlock.h
// Description: A simple class that contains information about a text block.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#pragma once

class TextBlock
{
public:
	TextBlock(void);
	int left;
	int right;
	int up;
	int down;
	char split;
};
