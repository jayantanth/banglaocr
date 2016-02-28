///////////////////////////////////////////////////////////////////////
// File:        ImageProcessor.cpp
// Description: Internal Image processor of Bangla OCR.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(bool** BArray, int height, int width, vector<UnitInformation> infoUnits, int temporaryFileNumber)
{
	ht = height;
	wd = width;

	// Initialize Img	
	vector<vector<bool>> Img(height, vector<bool>(width));

	for(int x=0; x<height; x++){
			for(int y=0; y<width; y++){
				Img[x][y] = true;
			}
	}

	for(int indx=0; indx<infoUnits.size(); indx++)
	{
		UnitInformation ui = infoUnits[indx];

		int left_ = ui.left;
		int right_ = ui.right; 
		int top_ = ui.top;
		int bottom_ = ui.bottom;

		for(int x=top_; x<=bottom_; x++){
			for(int y=left_; y<=right_; y++){
				if(!BArray[x][y])
					Img[x][y] = false;
			}
		}
	}

	Save_UC_TIF_ImageFromSegmentedInformation(Img, temporaryFileNumber);
	
	Img.~vector();
}

bool ImageProcessor::Save_UC_TIF_ImageFromSegmentedInformation(vector<vector<bool>> Img, int temporaryFileNumber){
	try{
		Bitmap^ nimg = gcnew Bitmap(wd, ht, PixelFormat::Format1bppIndexed);				 

		//lock the bits of the original bitmap
		BitmapData^ bmdo = nimg->LockBits(Rectangle(0, 0, wd, ht), ImageLockMode::ReadWrite, nimg->PixelFormat);

		for(int x=0; x<ht; x++){
			for(int y=0; y<wd; y++){
				if(Img[x][y]){
					this->SetIndexedPixel(y, x, bmdo, 1);
				}else
				{
					this->SetIndexedPixel(y, x, bmdo, 0);
				}
			}
		}

		//System::Drawing::Image::SetPropertyItem(
		nimg->UnlockBits(bmdo);

		// Now we need to set the compression information here
		ImageCodecInfo^ ico = GetEncoderInfo(ImageFormat::Tiff);

		// Create an Encoder object based on the GUID for the Compression parameter category.
		Encoder^ myEncoder = Encoder::Compression;

		// Create an EncoderParameters object. An EncoderParameters object has an array of EncoderParameter objects. 
		// In this case, there is only one EncoderParameter object in the array.
		EncoderParameters^ myEncoderParameters = gcnew EncoderParameters(1);

		EncoderParameter^ myEncoderParameter = gcnew EncoderParameter(myEncoder, (__int64)EncoderValue::CompressionNone);
		myEncoderParameters->Param[0] = myEncoderParameter;

		nimg->Save(System::Windows::Forms::Application::StartupPath + "\\Tesseract\\temp"+ temporaryFileNumber +".tif", ico, myEncoderParameters);

		return true;
	}
	catch(System::Exception^ ex)
	{
		System::Windows::Forms::MessageBox::Show(ex->Message->ToString(), "Failed to Process the Image!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
		return false;
	}
}

void ImageProcessor::SetIndexedPixel(int x,int y, System::Drawing::Imaging::BitmapData^ bmd, bool pixel)
			 {
				 try{
				 int index = y*bmd->Stride +(x>>3);

				 Byte p = System::Runtime::InteropServices::Marshal::ReadByte(bmd->Scan0, index);

				 Byte mask=(Byte)(0x80>>(x&0x7));

				 if(pixel){
					 p |=mask;
				 }	
				 else
				 {
					 p &=(Byte)(mask^0xff);
				 }

				 System::Runtime::InteropServices::Marshal::WriteByte(bmd->Scan0,index,p);
				 }
				 catch(System::Exception^ ex)
				 {
					 System::Windows::Forms::MessageBox::Show("Can not save image pixels in 1 bit per pixel format.","Failed to Process Image!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
					 return;
				 }
			 }

ImageCodecInfo^ ImageProcessor::GetEncoderInfo(System::Drawing::Imaging::ImageFormat^ imgFmt){
				 array<System::Drawing::Imaging::ImageCodecInfo^>^ codecs = System::Drawing::Imaging::ImageCodecInfo::GetImageDecoders();
				 int numCodecs = codecs->GetLength(0);

				 for(int i=0; i<numCodecs; i++){
					 if(codecs[i]->FormatID == imgFmt->Guid){
						 return codecs[i];
					 }
				 }
			 }

