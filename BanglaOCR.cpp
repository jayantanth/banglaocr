///////////////////////////////////////////////////////////////////////
// File:        BanglaOCR.cpp
// Description: Bangla OCR.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.6
///////////////////////////////////////////////////////////////////////


// BanglaOCR.cpp : main project file.

#include "stdafx.h"
#include "FormMainWindow.h"

using namespace BanglaOCR;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
