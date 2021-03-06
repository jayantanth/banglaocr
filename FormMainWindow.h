///////////////////////////////////////////////////////////////////////
// File:        FormMainWindow.h
// Description: Interface of Bangla OCR.
// Author:      Md. Abul Hasnat
// Organization Center for Research on Bangla Language Processing (CRBLP)
// Version:		0.7
///////////////////////////////////////////////////////////////////////

#include "ThresholedValue.h"
#include "rgbConvert.h"
#include "ImageProcessor.h"
#include "CCAnalyzer.h"
#include "LayoutAnalysis.h"
#include "HSegmenter.h"
#include "ConnCompProperties.h"
#include "SkewDetectionCorrection.h"
#include "PostProcessor.h"
#include "FormAbout.h"
#include<iostream>

#pragma once
using namespace std;

namespace BanglaOCR {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	using namespace System::IO;



	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			// Set this debug flag
			//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

			InitializeComponent();

			this->BinaryDone = false;
			this->SeparateDone = false;
			/* Different Path Addresses */
			// getting the application path
			this->applicationPath = Application::StartupPath->ToString();

			// for the directory, path appear an extra '/'
			// here we are removing that '/'
			int len = this->applicationPath->Length -1;			
			if(this->applicationPath->Substring(len)->Equals("\\"))
			{
				this->applicationPath = this->applicationPath->Substring(0,len);				
			}

			// saved text from the rich text box
			this->savedText = "";

			this->temporaryFileNumber = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBoxShow;
	//////// My Variable :)  /////////////////////////////////
	private: int thresholdValue;
 	private: bool isThValComputed;
	private: bool isGrayImageAvailable;
	private: bool BinaryDone;
	private: bool SkewDone;
	private: bool NoiseEliminationDone;
	private: bool SeparateDone;
		
	private: int xsize;
	private: int ysize;

	private: Bitmap^ im;
	private: Bitmap^ BinaryImage;
	private: Bitmap^ FinalImage;

	private: bool **BArray;
	private: bool **BlockImgArray;
	private: bool **SegImg;
	private: int** grayImageArr;

	private: Graphics^ g;	
	private: String^ applicationPath;

	private: System::Drawing::Point^ PointForProg;

	private: String^ savedText;

	// for recognition task
	private: System::Windows::Forms::RichTextBox^  richTextBoxOutput;
	private: System::Windows::Forms::Button^  buttonSave;
	private: System::Windows::Forms::Button^  buttonClose;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;

	// for the entire task record
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::Panel^  panelText;
	private: System::Windows::Forms::ToolStripContainer^  toolStripContainer1;
	private: System::Windows::Forms::ToolStripStatusLabel^  statusText;
	private: System::Windows::Forms::ToolStripButton^  toolStripButtonRecognize;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  buttonSpChk;
	private: System::Windows::Forms::ToolStripProgressBar^  toolStripProgressBar1;


	// spell checking 
	private: System::Collections::SortedList^ errWords;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStripSuggestion;
	private: System::Windows::Forms::ToolStripMenuItem^  sug1ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sug2ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sug3ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sug4ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  sug5ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  ignoreToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^  addToDictionaryToolStripMenuItem;
	private: System::ComponentModel::IContainer^  components;

	// suggestion generation and replacement
	private: int startIndex;
	private: int endIndex;
    private: int currentClickIndex;
	private: System::String^ currentWordToReplace;
	private: System::Windows::Forms::Button^  buttonImageFit;
	private: System::Windows::Forms::Button^  buttonImageVerticallyFit;
	private: System::Windows::Forms::Panel^  panelImageButtons;
	private: System::Windows::Forms::Button^  buttonZoomOut;
	private: System::Windows::Forms::Button^  buttonZoomIn;
	private: System::Windows::Forms::Button^  buttonImageHorizontallyFit;
	private: System::Windows::Forms::Button^  buttonImageOriginial;

	// Picture Box Related
	private: static int scaleX = 654;
    private: static int scaleY = 464;
	private: int zoomHeight;
	private: int zoomWidth;
	
	// Temporary file number Related
    private: int temporaryFileNumber;

    private: ThresholedValue^ ts;
	private: rgbConvert^ rgbC;
	HSegmenter^ hsg;
	ImageProcessor^ ip;
	private: System::String^ recognizerOutput;

	/////// end my variable :p ////////////////////////////////////////

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripButton^  toolStripButtonOpen;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialogImage;
	private: System::Windows::Forms::Panel^  panelDraw;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::CheckBox^  checkBoxLA;
	private: System::Windows::Forms::CheckBox^  checkBoxSkewed;
	private: System::Windows::Forms::CheckBox^  checkBoxNoisy;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ToolStripMenuItem^  recognizeToolStripMenuItem;

	protected: 

	private:

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pictureBoxShow = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->recognizeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButtonOpen = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButtonRecognize = (gcnew System::Windows::Forms::ToolStripButton());
			this->openFileDialogImage = (gcnew System::Windows::Forms::OpenFileDialog());
			this->panelDraw = (gcnew System::Windows::Forms::Panel());
			this->panelImageButtons = (gcnew System::Windows::Forms::Panel());
			this->buttonImageOriginial = (gcnew System::Windows::Forms::Button());
			this->buttonZoomOut = (gcnew System::Windows::Forms::Button());
			this->buttonImageFit = (gcnew System::Windows::Forms::Button());
			this->buttonZoomIn = (gcnew System::Windows::Forms::Button());
			this->buttonImageVerticallyFit = (gcnew System::Windows::Forms::Button());
			this->buttonImageHorizontallyFit = (gcnew System::Windows::Forms::Button());
			this->richTextBoxOutput = (gcnew System::Windows::Forms::RichTextBox());
			this->buttonSave = (gcnew System::Windows::Forms::Button());
			this->buttonClose = (gcnew System::Windows::Forms::Button());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->statusText = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripProgressBar1 = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->panelText = (gcnew System::Windows::Forms::Panel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->buttonSpChk = (gcnew System::Windows::Forms::Button());
			this->toolStripContainer1 = (gcnew System::Windows::Forms::ToolStripContainer());
			this->contextMenuStripSuggestion = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->sug1ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sug2ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sug3ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sug4ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sug5ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->ignoreToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->addToDictionaryToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->checkBoxLA = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxSkewed = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxNoisy = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxShow))->BeginInit();
			this->menuStrip1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->panelDraw->SuspendLayout();
			this->panelImageButtons->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->panelText->SuspendLayout();
			this->panel1->SuspendLayout();
			this->toolStripContainer1->BottomToolStripPanel->SuspendLayout();
			this->toolStripContainer1->ContentPanel->SuspendLayout();
			this->toolStripContainer1->SuspendLayout();
			this->contextMenuStripSuggestion->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBoxShow
			// 
			this->pictureBoxShow->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBoxShow->Location = System::Drawing::Point(0, 0);
			this->pictureBoxShow->Name = L"pictureBoxShow";
			this->pictureBoxShow->Size = System::Drawing::Size(654, 464);
			this->pictureBoxShow->TabIndex = 0;
			this->pictureBoxShow->TabStop = false;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
				this->editToolStripMenuItem, this->helpToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1272, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
				this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(100, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripSeparator1, 
				this->recognizeToolStripMenuItem});
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->editToolStripMenuItem->Text = L"Edit";
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(120, 6);
			// 
			// recognizeToolStripMenuItem
			// 
			this->recognizeToolStripMenuItem->Enabled = false;
			this->recognizeToolStripMenuItem->Name = L"recognizeToolStripMenuItem";
			this->recognizeToolStripMenuItem->Size = System::Drawing::Size(123, 22);
			this->recognizeToolStripMenuItem->Text = L"Recognize";
			this->recognizeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::recognizeToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aboutToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(40, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->aboutToolStripMenuItem->Text = L"About";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::aboutToolStripMenuItem_Click);
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripButtonOpen, 
				this->toolStripButtonRecognize});
			this->toolStrip1->Location = System::Drawing::Point(0, 24);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(1272, 25);
			this->toolStrip1->TabIndex = 2;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripButtonOpen
			// 
			this->toolStripButtonOpen->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->toolStripButtonOpen->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButtonOpen->Name = L"toolStripButtonOpen";
			this->toolStripButtonOpen->Size = System::Drawing::Size(37, 22);
			this->toolStripButtonOpen->Tag = L"Open Image";
			this->toolStripButtonOpen->Text = L"Open";
			this->toolStripButtonOpen->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageAboveText;
			this->toolStripButtonOpen->ToolTipText = L"Select an Image";
			this->toolStripButtonOpen->Click += gcnew System::EventHandler(this, &Form1::toolStripButtonOpen_Click);
			// 
			// toolStripButtonRecognize
			// 
			this->toolStripButtonRecognize->Enabled = false;
			this->toolStripButtonRecognize->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->toolStripButtonRecognize->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButtonRecognize->Name = L"toolStripButtonRecognize";
			this->toolStripButtonRecognize->Size = System::Drawing::Size(60, 22);
			this->toolStripButtonRecognize->Text = L"Recognize";
			this->toolStripButtonRecognize->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageAboveText;
			this->toolStripButtonRecognize->Click += gcnew System::EventHandler(this, &Form1::toolStripButtonRecognize_Click);
			// 
			// openFileDialogImage
			// 
			this->openFileDialogImage->FileName = L"open an Image";
			this->openFileDialogImage->Filter = L"JPG|*.jpg|BMP|*.bmp|GIF|*.gif|PNG|*.png|TIF|*.tif";
			// 
			// panelDraw
			// 
			this->panelDraw->AutoScroll = true;
			this->panelDraw->Controls->Add(this->pictureBoxShow);
			this->panelDraw->Location = System::Drawing::Point(35, 0);
			this->panelDraw->Name = L"panelDraw";
			this->panelDraw->Size = System::Drawing::Size(659, 467);
			this->panelDraw->TabIndex = 3;
			// 
			// panelImageButtons
			// 
			this->panelImageButtons->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panelImageButtons->Controls->Add(this->buttonImageOriginial);
			this->panelImageButtons->Controls->Add(this->buttonZoomOut);
			this->panelImageButtons->Controls->Add(this->buttonImageFit);
			this->panelImageButtons->Controls->Add(this->buttonZoomIn);
			this->panelImageButtons->Controls->Add(this->buttonImageVerticallyFit);
			this->panelImageButtons->Controls->Add(this->buttonImageHorizontallyFit);
			this->panelImageButtons->Location = System::Drawing::Point(3, 2);
			this->panelImageButtons->Name = L"panelImageButtons";
			this->panelImageButtons->Size = System::Drawing::Size(29, 211);
			this->panelImageButtons->TabIndex = 6;
			// 
			// buttonImageOriginial
			// 
			this->buttonImageOriginial->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(222)));
			this->buttonImageOriginial->Location = System::Drawing::Point(1, 178);
			this->buttonImageOriginial->Name = L"buttonImageOriginial";
			this->buttonImageOriginial->Size = System::Drawing::Size(25, 28);
			this->buttonImageOriginial->TabIndex = 6;
			this->buttonImageOriginial->Text = L"¤";
			this->buttonImageOriginial->UseVisualStyleBackColor = true;
			this->buttonImageOriginial->Click += gcnew System::EventHandler(this, &Form1::buttonImageOriginial_Click);
			// 
			// buttonZoomOut
			// 
			this->buttonZoomOut->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(222)));
			this->buttonZoomOut->Location = System::Drawing::Point(1, 144);
			this->buttonZoomOut->Name = L"buttonZoomOut";
			this->buttonZoomOut->Size = System::Drawing::Size(25, 28);
			this->buttonZoomOut->TabIndex = 5;
			this->buttonZoomOut->Text = L"-";
			this->buttonZoomOut->UseVisualStyleBackColor = true;
			this->buttonZoomOut->Click += gcnew System::EventHandler(this, &Form1::buttonZoomOut_Click);
			// 
			// buttonImageFit
			// 
			this->buttonImageFit->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->buttonImageFit->Location = System::Drawing::Point(1, 8);
			this->buttonImageFit->Name = L"buttonImageFit";
			this->buttonImageFit->Size = System::Drawing::Size(25, 28);
			this->buttonImageFit->TabIndex = 1;
			this->buttonImageFit->Text = L"╬";
			this->buttonImageFit->UseVisualStyleBackColor = true;
			this->buttonImageFit->Click += gcnew System::EventHandler(this, &Form1::buttonImageFit_Click);
			// 
			// buttonZoomIn
			// 
			this->buttonZoomIn->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(222)));
			this->buttonZoomIn->Location = System::Drawing::Point(1, 110);
			this->buttonZoomIn->Name = L"buttonZoomIn";
			this->buttonZoomIn->Size = System::Drawing::Size(25, 28);
			this->buttonZoomIn->TabIndex = 4;
			this->buttonZoomIn->Text = L"+";
			this->buttonZoomIn->UseVisualStyleBackColor = true;
			this->buttonZoomIn->Click += gcnew System::EventHandler(this, &Form1::buttonZoomIn_Click);
			// 
			// buttonImageVerticallyFit
			// 
			this->buttonImageVerticallyFit->Font = (gcnew System::Drawing::Font(L"Arial", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0), true));
			this->buttonImageVerticallyFit->Location = System::Drawing::Point(1, 42);
			this->buttonImageVerticallyFit->Name = L"buttonImageVerticallyFit";
			this->buttonImageVerticallyFit->Size = System::Drawing::Size(25, 28);
			this->buttonImageVerticallyFit->TabIndex = 2;
			this->buttonImageVerticallyFit->Text = L"↔";
			this->buttonImageVerticallyFit->UseVisualStyleBackColor = true;
			this->buttonImageVerticallyFit->Click += gcnew System::EventHandler(this, &Form1::buttonImageVerticallyFit_Click);
			// 
			// buttonImageHorizontallyFit
			// 
			this->buttonImageHorizontallyFit->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->buttonImageHorizontallyFit->Location = System::Drawing::Point(1, 76);
			this->buttonImageHorizontallyFit->Name = L"buttonImageHorizontallyFit";
			this->buttonImageHorizontallyFit->Size = System::Drawing::Size(25, 28);
			this->buttonImageHorizontallyFit->TabIndex = 3;
			this->buttonImageHorizontallyFit->Text = L"↕";
			this->buttonImageHorizontallyFit->UseVisualStyleBackColor = true;
			this->buttonImageHorizontallyFit->Click += gcnew System::EventHandler(this, &Form1::buttonImageHorizontallyFit_Click);
			// 
			// richTextBoxOutput
			// 
			this->richTextBoxOutput->Font = (gcnew System::Drawing::Font(L"SolaimanLipi", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->richTextBoxOutput->Location = System::Drawing::Point(12, 0);
			this->richTextBoxOutput->Name = L"richTextBoxOutput";
			this->richTextBoxOutput->Size = System::Drawing::Size(553, 433);
			this->richTextBoxOutput->TabIndex = 5;
			this->richTextBoxOutput->Text = L"";
			this->richTextBoxOutput->Visible = false;
			this->richTextBoxOutput->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::richTextBoxOutput_MouseUp);
			// 
			// buttonSave
			// 
			this->buttonSave->Font = (gcnew System::Drawing::Font(L"Times New Roman", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->buttonSave->Location = System::Drawing::Point(225, 2);
			this->buttonSave->Name = L"buttonSave";
			this->buttonSave->Size = System::Drawing::Size(95, 24);
			this->buttonSave->TabIndex = 6;
			this->buttonSave->Text = L"Save";
			this->buttonSave->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageBeforeText;
			this->buttonSave->UseVisualStyleBackColor = true;
			this->buttonSave->Click += gcnew System::EventHandler(this, &Form1::buttonSave_Click);
			// 
			// buttonClose
			// 
			this->buttonClose->Font = (gcnew System::Drawing::Font(L"Times New Roman", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->buttonClose->Location = System::Drawing::Point(326, 2);
			this->buttonClose->Name = L"buttonClose";
			this->buttonClose->Size = System::Drawing::Size(95, 24);
			this->buttonClose->TabIndex = 7;
			this->buttonClose->Text = L"Close";
			this->buttonClose->UseVisualStyleBackColor = true;
			this->buttonClose->Click += gcnew System::EventHandler(this, &Form1::buttonClose_Click);
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->Filter = L"DOC|*.doc|RTF|*.rtf";
			// 
			// statusStrip1
			// 
			this->statusStrip1->Dock = System::Windows::Forms::DockStyle::None;
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->statusText, this->toolStripProgressBar1});
			this->statusStrip1->Location = System::Drawing::Point(0, 0);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(1272, 22);
			this->statusStrip1->TabIndex = 8;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// statusText
			// 
			this->statusText->Name = L"statusText";
			this->statusText->Size = System::Drawing::Size(0, 17);
			// 
			// toolStripProgressBar1
			// 
			this->toolStripProgressBar1->Name = L"toolStripProgressBar1";
			this->toolStripProgressBar1->Size = System::Drawing::Size(100, 16);
			// 
			// panelText
			// 
			this->panelText->Controls->Add(this->richTextBoxOutput);
			this->panelText->Controls->Add(this->panel1);
			this->panelText->Location = System::Drawing::Point(700, 0);
			this->panelText->Name = L"panelText";
			this->panelText->Size = System::Drawing::Size(570, 467);
			this->panelText->TabIndex = 9;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->buttonSpChk);
			this->panel1->Controls->Add(this->buttonClose);
			this->panel1->Controls->Add(this->buttonSave);
			this->panel1->Location = System::Drawing::Point(143, 437);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(429, 29);
			this->panel1->TabIndex = 8;
			// 
			// buttonSpChk
			// 
			this->buttonSpChk->Font = (gcnew System::Drawing::Font(L"Times New Roman", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->buttonSpChk->Location = System::Drawing::Point(124, 2);
			this->buttonSpChk->Name = L"buttonSpChk";
			this->buttonSpChk->Size = System::Drawing::Size(95, 24);
			this->buttonSpChk->TabIndex = 8;
			this->buttonSpChk->Text = L"Check Spell";
			this->buttonSpChk->TextImageRelation = System::Windows::Forms::TextImageRelation::ImageBeforeText;
			this->buttonSpChk->UseVisualStyleBackColor = true;
			this->buttonSpChk->Click += gcnew System::EventHandler(this, &Form1::buttonSpChk_Click);
			// 
			// toolStripContainer1
			// 
			// 
			// toolStripContainer1.BottomToolStripPanel
			// 
			this->toolStripContainer1->BottomToolStripPanel->Controls->Add(this->statusStrip1);
			// 
			// toolStripContainer1.ContentPanel
			// 
			this->toolStripContainer1->ContentPanel->Controls->Add(this->panelImageButtons);
			this->toolStripContainer1->ContentPanel->Controls->Add(this->panelDraw);
			this->toolStripContainer1->ContentPanel->Controls->Add(this->panelText);
			this->toolStripContainer1->ContentPanel->Size = System::Drawing::Size(1272, 470);
			this->toolStripContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->toolStripContainer1->LeftToolStripPanelVisible = false;
			this->toolStripContainer1->Location = System::Drawing::Point(0, 49);
			this->toolStripContainer1->Name = L"toolStripContainer1";
			this->toolStripContainer1->RightToolStripPanelVisible = false;
			this->toolStripContainer1->Size = System::Drawing::Size(1272, 492);
			this->toolStripContainer1->TabIndex = 10;
			this->toolStripContainer1->Text = L"toolStripContainer1";
			this->toolStripContainer1->TopToolStripPanelVisible = false;
			// 
			// contextMenuStripSuggestion
			// 
			this->contextMenuStripSuggestion->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {this->sug1ToolStripMenuItem, 
				this->sug2ToolStripMenuItem, this->sug3ToolStripMenuItem, this->sug4ToolStripMenuItem, this->sug5ToolStripMenuItem, this->toolStripMenuItem1, 
				this->ignoreToolStripMenuItem, this->toolStripMenuItem2, this->addToDictionaryToolStripMenuItem});
			this->contextMenuStripSuggestion->Name = L"contextMenuStripSuggestion";
			this->contextMenuStripSuggestion->Size = System::Drawing::Size(177, 170);
			// 
			// sug1ToolStripMenuItem
			// 
			this->sug1ToolStripMenuItem->Font = (gcnew System::Drawing::Font(L"SolaimanLipi", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->sug1ToolStripMenuItem->Name = L"sug1ToolStripMenuItem";
			this->sug1ToolStripMenuItem->Size = System::Drawing::Size(176, 22);
			this->sug1ToolStripMenuItem->Text = L"sug1";
			this->sug1ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sug1ToolStripMenuItem_Click);
			// 
			// sug2ToolStripMenuItem
			// 
			this->sug2ToolStripMenuItem->Font = (gcnew System::Drawing::Font(L"SolaimanLipi", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->sug2ToolStripMenuItem->Name = L"sug2ToolStripMenuItem";
			this->sug2ToolStripMenuItem->Size = System::Drawing::Size(176, 22);
			this->sug2ToolStripMenuItem->Text = L"sug2";
			this->sug2ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sug2ToolStripMenuItem_Click);
			// 
			// sug3ToolStripMenuItem
			// 
			this->sug3ToolStripMenuItem->Font = (gcnew System::Drawing::Font(L"SolaimanLipi", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->sug3ToolStripMenuItem->Name = L"sug3ToolStripMenuItem";
			this->sug3ToolStripMenuItem->Size = System::Drawing::Size(176, 22);
			this->sug3ToolStripMenuItem->Text = L"sug3";
			this->sug3ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sug3ToolStripMenuItem_Click);
			// 
			// sug4ToolStripMenuItem
			// 
			this->sug4ToolStripMenuItem->Font = (gcnew System::Drawing::Font(L"SolaimanLipi", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->sug4ToolStripMenuItem->Name = L"sug4ToolStripMenuItem";
			this->sug4ToolStripMenuItem->Size = System::Drawing::Size(176, 22);
			this->sug4ToolStripMenuItem->Text = L"sug4";
			this->sug4ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sug4ToolStripMenuItem_Click);
			// 
			// sug5ToolStripMenuItem
			// 
			this->sug5ToolStripMenuItem->Font = (gcnew System::Drawing::Font(L"SolaimanLipi", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->sug5ToolStripMenuItem->Name = L"sug5ToolStripMenuItem";
			this->sug5ToolStripMenuItem->Size = System::Drawing::Size(176, 22);
			this->sug5ToolStripMenuItem->Text = L"sug5";
			this->sug5ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sug5ToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(173, 6);
			// 
			// ignoreToolStripMenuItem
			// 
			this->ignoreToolStripMenuItem->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ignoreToolStripMenuItem->Name = L"ignoreToolStripMenuItem";
			this->ignoreToolStripMenuItem->Size = System::Drawing::Size(176, 22);
			this->ignoreToolStripMenuItem->Text = L"Ignore";
			this->ignoreToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ignoreToolStripMenuItem_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(173, 6);
			// 
			// addToDictionaryToolStripMenuItem
			// 
			this->addToDictionaryToolStripMenuItem->Font = (gcnew System::Drawing::Font(L"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->addToDictionaryToolStripMenuItem->Name = L"addToDictionaryToolStripMenuItem";
			this->addToDictionaryToolStripMenuItem->Size = System::Drawing::Size(176, 22);
			this->addToDictionaryToolStripMenuItem->Text = L"Add To Dictionary";
			this->addToDictionaryToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::addToDictionaryToolStripMenuItem_Click);
			// 
			// checkBoxLA
			// 
			this->checkBoxLA->AutoSize = true;
			this->checkBoxLA->Location = System::Drawing::Point(591, 28);
			this->checkBoxLA->Name = L"checkBoxLA";
			this->checkBoxLA->Size = System::Drawing::Size(91, 17);
			this->checkBoxLA->TabIndex = 11;
			this->checkBoxLA->Text = L"Multi Columns";
			this->checkBoxLA->UseVisualStyleBackColor = true;
			// 
			// checkBoxSkewed
			// 
			this->checkBoxSkewed->AutoSize = true;
			this->checkBoxSkewed->Location = System::Drawing::Point(462, 28);
			this->checkBoxSkewed->Name = L"checkBoxSkewed";
			this->checkBoxSkewed->Size = System::Drawing::Size(65, 17);
			this->checkBoxSkewed->TabIndex = 12;
			this->checkBoxSkewed->Text = L"Skewed";
			this->checkBoxSkewed->UseVisualStyleBackColor = true;
			// 
			// checkBoxNoisy
			// 
			this->checkBoxNoisy->AutoSize = true;
			this->checkBoxNoisy->Location = System::Drawing::Point(533, 28);
			this->checkBoxNoisy->Name = L"checkBoxNoisy";
			this->checkBoxNoisy->Size = System::Drawing::Size(52, 17);
			this->checkBoxNoisy->TabIndex = 13;
			this->checkBoxNoisy->Text = L"Noisy";
			this->checkBoxNoisy->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(328, 29);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(129, 13);
			this->label1->TabIndex = 14;
			this->label1->Text = L"Document Properties:";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlLight;
			this->ClientSize = System::Drawing::Size(1272, 541);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->checkBoxNoisy);
			this->Controls->Add(this->checkBoxSkewed);
			this->Controls->Add(this->checkBoxLA);
			this->Controls->Add(this->toolStripContainer1);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->menuStrip1);
			this->Cursor = System::Windows::Forms::Cursors::Default;
			this->MainMenuStrip = this->menuStrip1;
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->Opacity = 0.99;
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"BanglaOCR";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxShow))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->panelDraw->ResumeLayout(false);
			this->panelImageButtons->ResumeLayout(false);
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->panelText->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->toolStripContainer1->BottomToolStripPanel->ResumeLayout(false);
			this->toolStripContainer1->BottomToolStripPanel->PerformLayout();
			this->toolStripContainer1->ContentPanel->ResumeLayout(false);
			this->toolStripContainer1->ResumeLayout(false);
			this->toolStripContainer1->PerformLayout();
			this->contextMenuStripSuggestion->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion

	private: void OpenImageFile () 
			 {
				 this->statusText->Text = "Open an Image..";
				 this->Update();
				 System::Windows::Forms::DialogResult d = this->openFileDialogImage->ShowDialog();
				 if (d == System::Windows::Forms::DialogResult::OK)
				 {			 
					 // if open an image
					 this->panelText->Visible = false;
					 this->Height = 575;
					 this->Width = 705;
					 this->richTextBoxOutput->Text = "";

					 this->toolStripProgressBar1->Value = 0;
					 BinaryDone = false;
					 isThValComputed = false;
					 isGrayImageAvailable = false;
					 SeparateDone = false;
					 SkewDone = false;
					 NoiseEliminationDone = false;
					 SeparateDone = false;

					 try{
						 // free image
						 delete im;

						 // creating a bitmap
						 im = gcnew Bitmap(this->openFileDialogImage->OpenFile()); 
					 }catch(Exception^ ex){
						 System::Windows::Forms::MessageBox::Show("Invalid Image, Message: "+ex->Message,"Invalid Image file!",System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);	
						 return;
					 }					

					 //showing the image
					 this->pictureBoxShow->Image = im;

					 // Picture Box Related Information initialization
					 this->pictureBoxShow->Dock = System::Windows::Forms::DockStyle::None;
					 this->pictureBoxShow->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Normal;
					 this->pictureBoxShow->Height=im->Height;
					 this->pictureBoxShow->Width=im->Width;
					 this->pictureBoxShow->Update();

					 // set the zoom factors
					 zoomHeight = im->Height;
					 zoomWidth = im->Width;

					 // Set the image global size
					 xsize = im->Height;
					 ysize = im->Width;

					 // enable the recognition access
					 this->recognizeToolStripMenuItem->Enabled = true;
					 this->toolStripButtonRecognize->Enabled = true;
					 this->panelImageButtons->Visible = true;
				 }// end of if()

				 this->statusText->Text = "Image..";
				 this->Update();	
			 }

	private: bool makeBinary()
			 {
				 if (this->BinaryDone)
					 return false;

				 if(!isThValComputed){
					 isThValComputed = computeThreshold();
				 }

				 try{
					 if(isThValComputed){
						 ///// convert grayscale to binary /////
						 rgbConvert^ rgb = gcnew rgbConvert(im);

						 // convert rgb to grayscale
						 grayImageArr = rgb->getGrayImageArray();
						 isGrayImageAvailable = true;

						 // convert grayscale to binary
						 BArray = rgb->GetBinaryArray(thresholdValue);
					 }else{
						 this->BinaryDone = false;
						 return false;
					 }
				 }
				 catch(System::Exception^ ex)
				 {
					 System::Windows::Forms::MessageBox::Show(ex->Message->ToString(),"Failed to binarize the image!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
					 this->BinaryDone = false;
					 return false;
				 }
				 
				 this->statusText->Text = "Binarization completed..";
				 this->Update();

				 return true;
			 }// end of function MakeBainary()

	private: bool performSkewCorrection(SkewDetectionCorrection^ skdc)
			 {
				 if (SkewDone)
					 return false;

				 try{
					 //// finding UpperEnvelope
					 skdc->findUpperEnvelope();	 

					 float angelSkew = skdc->GetSkewAngle();

					 // Skew Correction
					 int yIncrSingleSide = 0;
					 int rYsize;


					 float ang = 0 - angelSkew;
					 double angleRAD = Math::PI*ang/180.0;

					 float c = Math::Cos(angleRAD);
					 float s = Math::Sin(angleRAD);
					 rYsize  = (int)Math::Round(Math::Abs(xsize*s) + ysize*c);

					 yIncrSingleSide = (rYsize - ysize)/2;

					 if(yIncrSingleSide>0){
						 int ** grayArr;
						 grayArr = new int * [xsize];
						 for (int i = 0; i < xsize; i++)
						 {
							 grayArr[i] = new int [rYsize];
							 for (int j = 0; j < rYsize; j++)
							 {
								 grayArr[i][j] = 255;
							 }
						 }

						 for (int i = 0; i < xsize; i++)
						 {
							 for (int j = 0; j < ysize; j++)
							 {
								 grayArr[i][j+yIncrSingleSide] = grayImageArr[i][j];
							 }
						 }

						 this->grayImageArr = grayArr;
						 skdc->setYsize(rYsize);
						 this->ysize = rYsize;
					 }

					 // Show DeSkewed Image
					 if(angelSkew != 0.0){
						 skdc->setGrayImageArray(this->grayImageArr);

						 this->grayImageArr = skdc->RotateImgDirectInterpolate(angelSkew);

						 // Restore the binary array
						 ComputeBinaryArray();

						 // Re generate the binary image
						 this->getBainaryImage();

						 // Skew Correction
						 im = this->BinaryImage;
						 this->pictureBoxShow->Image = im;
						 this->pictureBoxShow->Update();
					 }

					 SkewDone = true;
				 }catch(System::Exception^ ex){
					 System::Windows::Forms::MessageBox::Show(ex->Message->ToString(),"Failed to deskew the image!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
					 SkewDone = false;
					 return false;
				 }

				 return true;
			 }// end of function performSkewCorrection()

	private: bool performNoiseElimination(int noiseSize)
			 {
				 if (NoiseEliminationDone)
					 return false;

				 try{
					 // Method - Adaptive Noise Removal : Using External Connected Component Analysis | NoiseRemove ///////
					 CCAnalyzer^ cca = gcnew CCAnalyzer(BArray, xsize, ysize, false);

					 // Get the connected components
					 vector<vector<point>> ccS = cca->labelCC_adv_mood();

					 // Get the properties of the connected components
					 ConnCompProperties ccp(ccS);
					 vector<vector<int>> regionsAll = ccp.getAllRegion();
					 int numOfComponent = regionsAll.size();

					 // Compute the width of the components
					 vector<int> CCWidth;
					 vector<int> CCHeight;
					 for(int rw=0; rw<numOfComponent; rw++){
						 CCWidth.push_back(regionsAll[rw][3] - regionsAll[rw][1] + 1);
						 CCHeight.push_back(regionsAll[rw][2] - regionsAll[rw][0] + 1);
					 }

					 // Compute mean
					 float meanWidth = 0;
					 float meanHeight = 0;	
					 for (int i = 0; i < numOfComponent; i++)
					 {
						 meanWidth += CCWidth[i];
						 meanHeight += CCHeight[i];
					 }
					 meanWidth /= numOfComponent;
					 meanHeight /= numOfComponent;

					 // Compute Standard Deviation
					 float stDevWidth = 0;
					 float stDevHeight = 0;

					 for (int i = 0; i < numOfComponent; i++)
					 {
						 stDevWidth += Math::Pow(CCWidth[i] - meanWidth, 2.0);
						 stDevHeight += Math::Pow(CCHeight[i] - meanHeight, 2.0);
					 }
					 stDevWidth /= numOfComponent;
					 stDevHeight /= numOfComponent;

					 stDevWidth = Math::Sqrt(stDevWidth);
					 stDevHeight = Math::Sqrt(stDevHeight);

					 vector<point> ccPoints;
					 int numPoints;
					 for (int i = 0; i < numOfComponent; i++)
					 {
						 ccPoints = ccS[i];
						 numPoints = ccPoints.size();

						 if (numPoints>noiseSize && CCHeight[i] <= meanHeight + 3*stDevHeight && CCWidth[i] <= meanWidth + 6*stDevWidth)
							 continue;

						 for(int cp=0; cp<numPoints; cp++){
							 BArray[ccPoints[cp].x][ccPoints[cp].y] = 1;
						 }
					 }

					 // Re generate the binary image
					 this->getBainaryImage();

					 // Skew Correction
					 im = this->BinaryImage;
					 this->pictureBoxShow->Image = im;
					 this->pictureBoxShow->Update();

					 NoiseEliminationDone = true;
				 }catch(System::Exception^ ex){
					 System::Windows::Forms::MessageBox::Show(ex->Message->ToString(),"Failed to eliminate noise!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
					 NoiseEliminationDone = false;
					 return false;
				 }

				 return true;
			 }// end of function performNoiseElimination()

	private: System::String^ RecognizeWord(System::Drawing::Bitmap^ tessImg)
			 {
				 // Perform Recognition by Tesseract and store result
				 System::String^ result = PerformRecognitionByTesseractEXE(tessImg);
				 result = result->Trim();

				 return result;
			 }

	private: bool CompleteRecognitionProcess()
			 {
				 // Task 1 : Binarization
				 // Task 2 : Skew Correction
				 // Task 3 : Noise Elimination
				 // Task 4 : Page Layout Analysis
				 // Task 5 : Segmentation
				 // Task 6 : Recognition

				 // Task 1 : Binarization
				 if(!BinaryDone){
					 // binarization...
					 if(!isThValComputed){
						 isThValComputed = computeThreshold();
				  }

					 bool binaryDoneStatus = makeBinary();

					 // binarization...
					 if(!isThValComputed && !binaryDoneStatus){
						 System::Windows::Forms::MessageBox::Show("Unable to perform binarization!!", "Failed in Binarization!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
						 return false;
				  }
				 }

				 this->toolStripProgressBar1->Value += 15;

				 if(checkBoxSkewed->Checked){
					 // Task 2 : Skew Correction and Detection
					 SkewDetectionCorrection^ skdc = gcnew SkewDetectionCorrection(BArray, im->Height, im->Width);
					 bool skewDoneStatus = performSkewCorrection(skdc);

					 if(!skewDoneStatus){
						 System::Windows::Forms::MessageBox::Show("Unable to perform skew correction!!", "Failed to proceed on skew correction!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
						 return false;
				  }

					 // status update
					 this->statusText->Text = "Skew Correction completed..";
					 this->toolStripProgressBar1->Value += 15;
					 this->Update();
				 }

				 if(checkBoxNoisy->Checked){
					 // Task 3 : Noise Elimination
					 int noiseSize = 6;
					 bool noiseEliminationDoneStatus = performNoiseElimination(noiseSize);

					 if(!noiseEliminationDoneStatus){
						 System::Windows::Forms::MessageBox::Show("Unable to perform Noise Elimination!!", "Failed to proceed on noise elimination!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
						 return false;
				  }

					 // status update
					 this->statusText->Text = "Noise elimination completed..";
					 this->toolStripProgressBar1->Value += 15;
					 this->Update();
				 }

				 // Task 4 : Page Layout Analysis
				 if(checkBoxLA->Checked){
					 LayoutAnalysis^ la = gcnew LayoutAnalysis(this->BArray, xsize, ysize);
					 vector<TextBlock> blocks = la->AnalysisLayout();
					 				 
					 // status update
					 this->statusText->Text = "Layout Analysis completed..";
					 this->toolStripProgressBar1->Value += 5;
					 this->Update();
					 
					 FinalImage = gcnew Bitmap(ysize, xsize);
					 for (int i=0; i<xsize; i++)
					 {
						 for (int j=0; j<ysize; j++)
						 {
							 FinalImage->SetPixel(j, i, Color::White);
						 }
					 }

					 // Now Process for each column
					 int numOfBlocks = blocks.size();

					 int blkImgHt;
					 int blkImgWd;
					 int lt;
					 int rt;
					 int tp;
					 int bm;
					 int ii;
					 int jj;
					 int blockHtThreshold = 10;
					 int blockWdThreshold = 10;

					 recognizerOutput = "";

					 int incrVal = (40/numOfBlocks)*0.5;
					 for(int bnm = 0; bnm<numOfBlocks; bnm++){
						 lt = blocks[bnm].left;	
						 rt = blocks[bnm].right;
						 tp = blocks[bnm].up;	
						 bm = blocks[bnm].down;

						 blkImgHt = bm - tp + 1;
						 blkImgWd = rt - lt + 1;

						 // Check the block height and width against threshold value
						 if(blkImgHt<= blockHtThreshold || blkImgWd<= blockWdThreshold){
							 continue;
						 }

						 //// build block image array	
						 BlockImgArray = new bool * [blkImgHt];
						 for (int i=tp,ni=0;i<=bm;i++,ni++)
						 {
							 BlockImgArray[ni] = new bool [blkImgWd];
							 for (int j=lt,nj=0;j<=rt;j++,nj++)
							 {
								 BlockImgArray[ni][nj] = BArray[i][j];
							 }
						 }						 

						 // status update
						 this->statusText->Text = "Segmentation of block number:"+ bnm +" started..";
						 this->Update();

						 // Task 5 : Segmentation
						 hsg = gcnew HSegmenter(BlockImgArray, blkImgHt, blkImgWd, bnm);		
						 vector<UnitInformation> infoUnits = hsg->SegmentWords();
						 this->toolStripProgressBar1->Value += incrVal;
						 this->Update();

						 if(!hsg->segmentSuccess){
							 System::Windows::Forms::MessageBox::Show("Unable to perform segmentation!!", "Failed to proceed on segmentation!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
							 return false;
						 }

						 // status update
						 this->statusText->Text = "Segmentation of block number:"+ bnm +" completed..";
						 this->Update();

						 // Save this image into the required image format of tesseract	using segmentation information
						 this->temporaryFileNumber = this->temporaryFileNumber % 20;
						 this->temporaryFileNumber += 1;
						 ip = gcnew ImageProcessor(BlockImgArray, blkImgHt, blkImgWd, infoUnits, this->temporaryFileNumber);

						 // Start Recognition (by tesseract)
						 im = gcnew System::Drawing::Bitmap(Application::StartupPath + "\\Tesseract\\temp"+ this->temporaryFileNumber +".tif");

						 for (int i=tp,ni=0;i<=bm;i++,ni++)
						 {
							 for (int j=lt,nj=0;j<=rt;j++,nj++)
							 {
								 FinalImage->SetPixel(j, i, im->GetPixel(nj, ni));
							 }
						 }

						 //showing the image units
						 this->pictureBoxShow->Image = FinalImage;
						 this->pictureBoxShow->Update();

						 // status update
						 this->statusText->Text = "Recognition of block number:"+ bnm +" started..";
						 this->Update();

						 // Task 6 : Recognition
						 recognizerOutput = recognizerOutput + "\n" +RecognizeWord(im);	

						 // status update
						 this->statusText->Text = "Recognition of block number:"+ bnm +" completed..";
						 this->toolStripProgressBar1->Value += incrVal; 
						 this->Update();

						 // free resources
						 infoUnits.clear();	
						 hsg->~HSegmenter();
					 }
					 this->toolStripProgressBar1->Value = 95;
					 this->statusText->Text = "Recognition Process End..";
					 this->Update();
				 }else{

					 // status update
					 this->statusText->Text = "Performing Segmentation..";
					 this->Update();

					 // Task 5 : Segmentation
					 hsg = gcnew HSegmenter(this->BArray, xsize, ysize);		
					 vector<UnitInformation> infoUnits = hsg->SegmentWords();
					 this->toolStripProgressBar1->Value = 40;
					 this->Update();

					 if(!hsg->segmentSuccess){
						 System::Windows::Forms::MessageBox::Show("Unable to perform segmentation!!", "Failed to proceed on segmentation!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
						 return false;
					 }

					 // status update
					 this->statusText->Text = "Segmentation completed..";
					 this->Update();

					 // Save this image into the required image format of tesseract	using segmentation information
					 this->temporaryFileNumber = this->temporaryFileNumber % 20;
					 this->temporaryFileNumber += 1;
					 ip = gcnew ImageProcessor(this->BArray, xsize, ysize, infoUnits, this->temporaryFileNumber);

					 this->toolStripProgressBar1->Value = 50;

					 // Start Recognition (by tesseract)
					 im = gcnew System::Drawing::Bitmap(Application::StartupPath + "\\Tesseract\\temp"+ this->temporaryFileNumber +".tif");

					 //showing the image units
					 this->pictureBoxShow->Image = im;
					 this->pictureBoxShow->Update();

					 // status update
					 this->statusText->Text = "Recognition started..";
					 this->Update();

					 // Task 6 : Recognition
					 recognizerOutput = RecognizeWord(im);

					 this->toolStripProgressBar1->Value = 95;
					 this->statusText->Text = "Recognition Process End..";
					 this->Update();

					 // free resources
					 infoUnits.clear();
				 }
				 return true;
			 }
	private: System::String^ PerformRecognitionByTesseractEXE(System::Drawing::Bitmap^ tessImg)
			 {
				 System::String^ strRes = "";
				 //////
				 System::String^ execDir  = this->applicationPath + "\\Tesseract";
				 System::String^ execDrive = execDir->Substring(0,execDir->IndexOf(":")+1);
				 System::String^ outputFile  = this->applicationPath + "\\Tesseract\\out.txt";
				 System::String^ tempTessFile  = this->applicationPath + "\\Tesseract\\tesseract.log";
				 System::String^ batchFile  = this->applicationPath + "\\Tesseract\\bat.bat";

				 try
				 {
					 // build and execute the batch file
					 System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(batchFile);
					 System::String^ tempStr = "";

					 // building the associate command for the initialization
					 tempStr += execDrive + "\n";
					 tempStr += "cd " + execDir + "\n";			// required for training from single sample
					 tempStr += "tesseract " + "temp" + this->temporaryFileNumber + ".tif" + " " + "out" + " " + "-l" + " " + "ban";

					 sw->Write(tempStr);
					 sw->Close();

					 // create a process and execute 
					 System::Diagnostics::ProcessStartInfo^ psInfo = gcnew System::Diagnostics::ProcessStartInfo(batchFile);
					 psInfo->CreateNoWindow = true;
					 psInfo->WindowStyle = System::Diagnostics::ProcessWindowStyle::Hidden;
					 System::Diagnostics::Process^ p = gcnew System::Diagnostics::Process();
					 p->StartInfo = psInfo;
					 p->Start();
					 p->WaitForExit();					 
				 }
				 catch (System::Exception^ excp)
				 {
					 System::Windows::Forms::MessageBox::Show(excp->Message->ToString(),"Can't Create/Execute Viterbi Decoder!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
					 exit(0);
				 }	

				 // read the outputfile
				 System::String^ tempStr = "";

				 try
				 {
					 System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(outputFile);
					 while (!System::String::IsNullOrEmpty((tempStr = sr->ReadLine())))
					 {
						 strRes += " " + tempStr;
					 }
					 sr->Close();

					 // Delete the temporary files
					 System::IO::File::Delete(batchFile);
					 System::IO::File::Delete(outputFile);
					 System::IO::File::Delete(tempTessFile);
				 }
				 catch(System::Exception^ ex)
				 {
					 System::Windows::Forms::MessageBox::Show(ex->Message->ToString(),"Unable to delete the temporary files!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
					 exit(0);
				 }
				 ///////
				 return strRes;
			 }

	private: void deleteTemporaryImageFilesUsingCMD(){
				 System::String^ batchFile  = this->applicationPath + "\\Tesseract\\tmfFileRemove.bat";
				 System::String^ execDir  = this->applicationPath + "\\Tesseract";
				 System::String^ execDrive = execDir->Substring(0,execDir->IndexOf(":")+1);
				 System::IO::StreamWriter^ sw;

				 try
				 {
					 // build and execute the batch file
					 sw = gcnew System::IO::StreamWriter(batchFile);
					 System::String^ tempStr = "";

					 // building the associate command for the initialization
					 tempStr += execDrive + "\n";
					 tempStr += "cd " + execDir + "\n";			// required for training from single sample
					 tempStr += "del " + "*.tif";

					 sw->Write(tempStr);
					 sw->Close();

					 // create a process and execute 
					 System::Diagnostics::ProcessStartInfo^ psInfo = gcnew System::Diagnostics::ProcessStartInfo(batchFile);
					 psInfo->CreateNoWindow = true;
					 psInfo->WindowStyle = System::Diagnostics::ProcessWindowStyle::Hidden;
					 System::Diagnostics::Process^ p = gcnew System::Diagnostics::Process();
					 p->StartInfo = psInfo;
					 p->Start();
					 p->WaitForExit();	
				 }
				 catch (System::Exception^ excp)
				 {
					 System::Windows::Forms::MessageBox::Show(excp->Message->ToString(),"Can't Create/Execute Viterbi Decoder!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
					 exit(0);
				 }

				 // Delete the batch file
				 try{
					 if(File::Exists(batchFile)){
						 File::Delete(batchFile);
					 }
				 }
				 catch (System::Exception^ excp){
				 }
			 }
	private: void deleteTemporaryImageFiles()
			 {	
				 try{
					 System::String^ tempFileName = "";
					 for(int i=1; i<=20; i++){
						tempFileName = Application::StartupPath + "\\Tesseract\\temp" + i + ".tif";
						if(File::Exists(tempFileName)){
							File::Delete(tempFileName);
						}
					 }
				 }
				 catch(System::Exception^ ex)
				 {
					 System::Windows::Forms::MessageBox::Show("Problem in deleting temporary image files. Please restart the application., Message: "+ex->Message,"Failed to Process the Image!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
					 return;
				 }
			 }

	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 deleteTemporaryImageFilesUsingCMD();
				 exit(0);
			 }
	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->OpenImageFile();
			 }

	private: System::Void toolStripButtonOpen_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->OpenImageFile();
			 }
	private: System::Void makeBinaryToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->makeBinary();
			 }
	private: System::Void toolStripButtonMakeBinary_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->makeBinary();
			 }

	private: System::Void recognizeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(this->CompleteRecognitionProcess()){
					 this->recognizeToolStripMenuItem->Enabled = false;
					 this->toolStripButtonRecognize->Enabled = false;
					 this->PerformPostProcessing();
				 }

				 // free resources
				 rgbC->~rgbConvert();
				 hsg->~HSegmenter();
			 }

	private: void PerformPostProcessing(){
				 // Perform Post Processing Task
				 this->statusText->Text = "Post-processing Started..";
				 this->Update();

				 /*System::IO::StreamReader^ sr = gcnew System::IO::StreamReader("C:\\out.txt");
				 System::String^ str = "";
				 str = sr->ReadLine();
				 while(!System::String::IsNullOrEmpty(str)){
					 recognizerOutput += str;
					 str = sr->ReadLine();
				 }
				 sr->Close();*/

				 PostProcessor^ pr = gcnew PostProcessor(this->recognizerOutput);
				 this->richTextBoxOutput->Text = pr->TextPostProcessor2();

				 this->statusText->Text = "Post-processing End..";
				 this->toolStripProgressBar1->Value = 100;
				 this->statusText->Text = "Completed OCR process..";
				 this->Update();

				 // Set option of the interface
				 this->richTextBoxOutput->Visible = true;
				 this->recognizeToolStripMenuItem->Enabled = false;
				 this->Height = 575;
				 this->Width = 1280;
				 this->panelText->Visible = true;
				 this->buttonSpChk->Text = "Check Spell";

				 this->Update();

				 // free resources
				 pr->~PostProcessor();
				 delete pr;
			 }

	private: System::Void buttonSave_Click(System::Object^  sender, System::EventArgs^  e) {
				 try{
					 this->saveFileDialog1->ShowDialog();
					 System::String^ fileText = this->richTextBoxOutput->Text;
					 System::String^ filePath = this->saveFileDialog1->FileName;

					 System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(filePath,false,System::Text::Encoding::UTF8);
					 sw->Write(fileText);
					 sw->Close();

					 System::Windows::Forms::MessageBox::Show("File Saved Successfully","Saved Successfully!",System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Exclamation);
				 }
				 catch(System::Exception^ ex)
				 {
					 return;
				 }
			 }
	private: System::Void buttonClose_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->panelText->Visible = false;
				 this->Height = 575;
				 this->Width = 705;
				 this->richTextBoxOutput->Text = "";
			 }
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->panelText->Visible = false;
				 this->panelImageButtons->Visible = false;
				 this->Width = 705;
				 this->Height = 575;

				 // delete previous temporary image files (this is a temporary solution as we are experiencing the problem : A generic error occured in GDI+)
				 deleteTemporaryImageFiles();
			 }
	private: System::Void toolStripButtonRecognize_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Cursor = System::Windows::Forms::Cursors::WaitCursor;

				 if(this->CompleteRecognitionProcess()){
					 this->recognizeToolStripMenuItem->Enabled = false;
					 this->toolStripButtonRecognize->Enabled = false;
					 this->PerformPostProcessing();
				 }

				 this->Cursor = System::Windows::Forms::Cursors::Default;
			 }
	private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 FormAbout ^fa = gcnew FormAbout();
				 fa->ShowDialog();
			 }
	private: System::Void originalImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->pictureBoxShow->Image = im;
			 }
	private: System::Void blackWhiteImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->pictureBoxShow->Image = this->BinaryImage;
			 }
	private: System::Void buttonSpChk_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Cursor = System::Windows::Forms::Cursors::WaitCursor;
				 checkSpell();
				 this->Cursor = System::Windows::Forms::Cursors::Default;
			 }
	private: void checkSpell()
			 {
				 System::String^ buttonText = buttonSpChk->Text;

				 if(buttonText->Equals("Check Spell")){
					 System::String^ inFile  = this->applicationPath + "\\SpellChecker\\words.txt";
					 System::String^ outFile  = this->applicationPath + "\\SpellChecker\\out.txt";
					 System::String^ dictFile  = this->applicationPath + "\\SpellChecker\\dictionary.txt";
					 System::String^ codeFile  = this->applicationPath + "\\SpellChecker\\codefile.code";
					 System::String^ execDir  = this->applicationPath + "\\SpellChecker";
					 System::String^ execDrive = execDir->Substring(0,execDir->IndexOf(":")+1);

					 System::String^ batchFile  = this->applicationPath + "\\SpellChecker\\bat.bat";

					 try
					 {
						 // save the text into a text file
						 //this->richTextBoxOutput->SaveFile(inFile,System::Windows::Forms::RichTextBoxStreamType::RichText);
						 System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(inFile,false,System::Text::Encoding::UTF8);
						 this->savedText = richTextBoxOutput->Text;	
						 sw->Write(savedText);
						 sw->Close();

						 // build and execute the batch file

						 sw = gcnew System::IO::StreamWriter(batchFile);
						 System::String^ tempStr = "";

						 // building the associate command for the initialization
						 tempStr += execDrive + "\n";
						 tempStr += "cd " + execDir + "\n";			// required for training from single sample
						 tempStr += "java -jar OCR.jar " + "words.txt" + " " + "out.txt" + " " + "dictionary.txt" + " " + "codefile.code";

						 sw->Write(tempStr);
						 sw->Close();

						 // create a process and execute 
						 System::Diagnostics::ProcessStartInfo^ psInfo = gcnew System::Diagnostics::ProcessStartInfo(batchFile);
						 psInfo->CreateNoWindow = true;
						 psInfo->WindowStyle = System::Diagnostics::ProcessWindowStyle::Hidden;
						 System::Diagnostics::Process^ p = gcnew System::Diagnostics::Process();
						 p->StartInfo = psInfo;
						 p->Start();
						 p->WaitForExit();	
					 }
					 catch (System::Exception^ excp)
					 {
						 System::Windows::Forms::MessageBox::Show(excp->Message->ToString(),"Can't Create/Execute Viterbi Decoder!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
						 exit(0);
					 }	

					 // read the outputfile
					 System::String^ tempStr = "";
					 System::String^ chkedStr = "";
					 System::String^ key = "";
					 System::String^ value = "";

					 // build a sorted list of the wrong words
					 errWords = gcnew System::Collections::SortedList();

					 try
					 {
						 System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(outFile);
						 while (!System::String::IsNullOrEmpty((tempStr = sr->ReadLine())))
						 {
							 chkedStr += tempStr;
							 if(tempStr->Contains(" "))
							 {
								 key = tempStr->Substring(0,tempStr->IndexOf(" "));
								 value = tempStr->Substring(tempStr->IndexOf(" ")+1);
								 if(!errWords->Contains(key))
								 {
									 errWords->Add(key,value);
								 }
							 }
						 }
						 sr->Close();
					 }
					 catch(System::Exception^ ex)
					 {
						 System::Windows::Forms::MessageBox::Show(ex->Message->ToString(),"Can't load the Unicodes!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
						 exit(0);
					 }

					 // show this to the text box
					 String^ str = this->richTextBoxOutput->Text;
					 array<Char>^chars = {' ',',',';',':'};
					 array<String^>^split = str->Split( chars );
					 IEnumerator^ myEnum = split->GetEnumerator();
					 int indx = 0;
					 while ( myEnum->MoveNext() )
					 {
						 String^ s = safe_cast<String^>(myEnum->Current);
						 if (  !s->Trim()->Equals( "" ) && errWords->Contains(s) )
						 {
							 String ^str = errWords->GetByIndex(errWords->IndexOfKey(s))->ToString();
							 //System::Windows::Forms::MessageBox::Show(str);
							 if(str->Trim() == "*")
							 {
								 indx = this->richTextBoxOutput->Text->IndexOf( s, indx );
								 this->richTextBoxOutput->Select(indx, s->Length);
								 this->richTextBoxOutput->SelectionColor = System::Drawing::Color::Red;
								 indx += s->Length;
							 }
							 else
							 {
								 indx = this->richTextBoxOutput->Text->IndexOf(s, indx );
								 this->richTextBoxOutput->Select(indx, s->Length);
								 this->richTextBoxOutput->SelectionColor = System::Drawing::Color::Green;
								 indx += s->Length;
							 }
						 }

					 }

					 // delete the temporary files
					 try
					 {
						 if(File::Exists(batchFile)){
							 File::Delete(batchFile);
						 }

						 if(File::Exists(inFile)){
							 File::Delete(inFile);
						 }

						 if(File::Exists(outFile)){
							 File::Delete(outFile);
						 }
					 }
					 catch(System::Exception^ ex)
					 {
						 System::Windows::Forms::MessageBox::Show(ex->Message->ToString(),"Can delete temporary files!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
						 exit(0);
					 }

					 // change the button text
					 this->buttonSpChk->Text = "Undo";
				 }
				 else
				 {
					 richTextBoxOutput->Text = savedText;	

					 // change the button text
					 this->buttonSpChk->Text = "Check Spell";
				 }
			 }
	private: System::Void richTextBoxOutput_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if(e->Button == System::Windows::Forms::MouseButtons::Right && !this->buttonSpChk->Text->Equals("Check Spell"))
				 {
					 String^ outputText = richTextBoxOutput->Text;
					 int charPos = this->richTextBoxOutput->GetCharIndexFromPosition(System::Drawing::Point(e->X, e->Y));
					 this->currentClickIndex = charPos;

					 // For character comparison purpose		 
					 cli::array<wchar_t,1>^ ch;
					 const wchar_t dari = 2404;
					 const wchar_t space = 32;
					 const wchar_t newLine = 10;
					 const wchar_t comma = 44;
					 const wchar_t invComma = 8217;
					 const wchar_t semiColon = 59;
					 const wchar_t hiPhen = 45;
					 
					 int stIndx = 1;
					 int endIndx = outputText->Length;
					 String^ currWord="";
					 bool proceedStatus;

					 try{
						 if(charPos != 0 && charPos != outputText->Length){
							 String^ charatPos = outputText->Substring(charPos-1, 1);

							 //set stIndx
							 stIndx = 0;
							 for(int cbs=charPos-1; cbs>=0; cbs--){
								 ch = outputText->Substring(cbs, 1)->ToCharArray(0,1);
								 if(ch[0]==space || ch[0]==newLine || ch[0]==dari || ch[0]==comma || ch[0]==invComma || ch[0]==semiColon || ch[0]==hiPhen){
									 stIndx = cbs+1;
									 break;
								 }
							 }

							 //set endIndx
							 endIndx = outputText->Length-1;
							 for(int cbs=charPos; cbs<outputText->Length; cbs++){
								 ch = outputText->Substring(cbs, 1)->ToCharArray(0,1);
								 if(ch[0]==space || ch[0]==newLine || ch[0]==dari || ch[0]==comma || ch[0]==invComma || ch[0]==semiColon || ch[0]==hiPhen){
									 endIndx = cbs-1;
									 break;
								 }
							 }
						 }else if(charPos == 0 && charPos != outputText->Length){
							 stIndx = 0;

							 //set endIndx
							 endIndx = outputText->Length-1;
							 for(int cbs=charPos; cbs<outputText->Length; cbs++){
								 ch = outputText->Substring(cbs, 1)->ToCharArray(0,1);
								 if(ch[0]==space || ch[0]==newLine || ch[0]==dari || ch[0]==comma || ch[0]==invComma || ch[0]==semiColon || ch[0]==hiPhen){
									 endIndx = cbs-1;
									 break;
								 }
							 }
						 }else if(charPos != 0 && charPos == outputText->Length){
							 //set stIndx
							 stIndx = 0;
							 for(int cbs=charPos-1; cbs>=0; cbs--){
								 ch = outputText->Substring(cbs, 1)->ToCharArray(0,1);
								 if(ch[0]==space || ch[0]==newLine || ch[0]==dari || ch[0]==comma || ch[0]==invComma || ch[0]==semiColon || ch[0]==hiPhen){
									 stIndx = cbs+1;
									 break;
								 }
							 }

							 endIndx = outputText->Length-1;
						 }

						 proceedStatus = false;
						 if(stIndx != endIndx){
							 proceedStatus = true;
							 currWord = outputText->Substring(stIndx, endIndx-stIndx+1);
						 }
					 }catch(Exception^ ex){
						 System::Windows::Forms::MessageBox::Show("Problem in locating word boundary.","Failed to determine word boundary!",System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);	
						 return;
					 }

					 if(proceedStatus){
						 if (errWords->Contains(currWord))
						 {
							 String ^str = errWords->GetByIndex(errWords->IndexOfKey(currWord))->ToString()->Trim();
							 this->setSuggestion(str);

							 // set the global suggestion related variables
							 this->currentWordToReplace = currWord;
							 this->startIndex = stIndx;
							 this->endIndex = endIndx;

							 // show the pop up menu	
							 int x = this->richTextBoxOutput->Location.X + this->Location.X + e->X+460;
							 int y = this->richTextBoxOutput->Location.Y + this->Location.Y + e->Y+90;
							 this->contextMenuStripSuggestion->Show(x, y);
						 }	
					 }
				 }
			 }

	private: void checkDocumentToCorrectMoreSimilarWords(){
				 String^ outputText = richTextBoxOutput->Text;

				 int stIndx;
				 int stringSearchStartIndex = 0;

				 bool haveMoreWord = true;

				 try{
					 while(haveMoreWord){
						 stIndx = outputText->IndexOf(this->currentWordToReplace, stringSearchStartIndex);
						 if(stIndx >= 0){
							 this->richTextBoxOutput->Select(stIndx, this->currentWordToReplace->Length);
							 this->richTextBoxOutput->SelectionColor = System::Drawing::Color::Black;
							 stringSearchStartIndex = stIndx + this->currentWordToReplace->Length - 1; 
						 }else{
							 haveMoreWord = false;
						 }
					 }
				 }catch(Exception^ ex){
					 System::Windows::Forms::MessageBox::Show("Problem in searching similar words.","Failed to find similar words!",System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);	
					 return;
				 }
			 }
	private: void setSuggestion(String^ str)
		 {
			 array<Char>^chars = {' '};
			 array<String^>^split = str->Split( chars );
			 int numOfSuggestion = split->Length;

			 String^ s = "";

			 // Manually set number of suggestion
			 if(numOfSuggestion == 1){
				s = safe_cast<String^>(split->GetValue(0));		
				if(s->Equals("*")){
					// make the suggestion container menu bar invisible
					this->sug1ToolStripMenuItem->Visible = false;
					this->sug2ToolStripMenuItem->Visible = false;
					this->sug3ToolStripMenuItem->Visible = false;
					this->sug4ToolStripMenuItem->Visible = false;
					this->sug5ToolStripMenuItem->Visible = false;
				}else{
					// suggestion 1
					this->sug1ToolStripMenuItem->Text = s;
					this->sug1ToolStripMenuItem->Visible = true;

					// make the rest of the suggestion container menu bar invisible
					this->sug2ToolStripMenuItem->Visible = false;
					this->sug3ToolStripMenuItem->Visible = false;
					this->sug4ToolStripMenuItem->Visible = false;
					this->sug5ToolStripMenuItem->Visible = false;
				}
			 }else if(numOfSuggestion == 2){
				 // suggestion 1
				 s = safe_cast<String^>(split->GetValue(0));
				 this->sug1ToolStripMenuItem->Text = s;
				 this->sug1ToolStripMenuItem->Visible = true;
				 // suggestion 2
				 s = safe_cast<String^>(split->GetValue(1));
				 this->sug2ToolStripMenuItem->Text = s;
				 this->sug2ToolStripMenuItem->Visible = true;

				 // make the rest of the suggestion container menu bar invisible
				 this->sug3ToolStripMenuItem->Visible = false;
				 this->sug4ToolStripMenuItem->Visible = false;
				 this->sug5ToolStripMenuItem->Visible = false;
			 }else if(numOfSuggestion == 3){
				 // suggestion 1
				 s = safe_cast<String^>(split->GetValue(0));
				 this->sug1ToolStripMenuItem->Text = s;
				 this->sug1ToolStripMenuItem->Visible = true;
				 // suggestion 2
				 s = safe_cast<String^>(split->GetValue(1));
				 this->sug2ToolStripMenuItem->Text = s;
				 this->sug2ToolStripMenuItem->Visible = true;
				 // suggestion 3
				 s = safe_cast<String^>(split->GetValue(2));
				 this->sug3ToolStripMenuItem->Text = s;
				 this->sug3ToolStripMenuItem->Visible = true;

				 // make the rest of the suggestion container menu bar invisible
				 this->sug4ToolStripMenuItem->Visible = false;
				 this->sug5ToolStripMenuItem->Visible = false;
			 }else if(numOfSuggestion == 4){
				 // suggestion 1
				 s = safe_cast<String^>(split->GetValue(0));
				 this->sug1ToolStripMenuItem->Text = s;
				 this->sug1ToolStripMenuItem->Visible = true;
				 // suggestion 2
				 s = safe_cast<String^>(split->GetValue(1));
				 this->sug2ToolStripMenuItem->Text = s;
				 this->sug2ToolStripMenuItem->Visible = true;
				 // suggestion 3
				 s = safe_cast<String^>(split->GetValue(2));
				 this->sug3ToolStripMenuItem->Text = s;
				 this->sug3ToolStripMenuItem->Visible = true;
				 // suggestion 4
				 s = safe_cast<String^>(split->GetValue(3));
				 this->sug4ToolStripMenuItem->Text = s;
				 this->sug4ToolStripMenuItem->Visible = true;

				 // make the rest of the suggestion container menu bar invisible
				 this->sug5ToolStripMenuItem->Visible = false;
			 }else if(numOfSuggestion >= 5){
				 // suggestion 1
				 s = safe_cast<String^>(split->GetValue(0));
				 this->sug1ToolStripMenuItem->Text = s;
				 this->sug1ToolStripMenuItem->Visible = true;
				 // suggestion 2
				 s = safe_cast<String^>(split->GetValue(1));
				 this->sug2ToolStripMenuItem->Text = s;
				 this->sug2ToolStripMenuItem->Visible = true;
				 // suggestion 3
				 s = safe_cast<String^>(split->GetValue(2));
				 this->sug3ToolStripMenuItem->Text = s;
				 this->sug3ToolStripMenuItem->Visible = true;
				 // suggestion 4
				 s = safe_cast<String^>(split->GetValue(3));
				 this->sug4ToolStripMenuItem->Text = s;
				 this->sug4ToolStripMenuItem->Visible = true;
				 // suggestion 5
				 s = safe_cast<String^>(split->GetValue(4));
				 this->sug5ToolStripMenuItem->Text = s;
				 this->sug5ToolStripMenuItem->Visible = true;
			 }		 
		 }// end of function setSuggestion

	private: void replaceWord(String ^str)
		 {
			 this->richTextBoxOutput->Select(this->startIndex, this->currentWordToReplace->Length);
			 this->richTextBoxOutput->SelectionColor = System::Drawing::Color::Black;
			 this->richTextBoxOutput->SelectedText = str;
			 this->richTextBoxOutput->Select(this->currentClickIndex, 0);	
		 }

	private: void ignoreWord()
		 {
			 this->richTextBoxOutput->Select(this->startIndex, this->currentWordToReplace->Length);
			 this->richTextBoxOutput->SelectionColor = System::Drawing::Color::Black;
			 this->richTextBoxOutput->Select(this->currentClickIndex, 0);
		 }
	private: void addToDictionary()
		 {
			 System::String^ dictFile = Application::StartupPath->ToString() + "\\SpellChecker\\dictionary.txt";
			 System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(dictFile,true,System::Text::Encoding::UTF8);
			 sw->Write(this->currentWordToReplace + "\n");
			 sw->Close();

			 // check the whole document to find more instance of the same word
			 checkDocumentToCorrectMoreSimilarWords();

			 // delete this word from the error word list
			 this->errWords->Remove(this->currentWordToReplace);

			 // set the cursor to the actual location
			 this->richTextBoxOutput->Select(this->currentClickIndex, 0);
		 }
	private: System::Void sug1ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 replaceWord(sug1ToolStripMenuItem->Text);
		 }
	private: System::Void sug2ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 replaceWord(sug2ToolStripMenuItem->Text);
		 }
	private: System::Void sug3ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 replaceWord(sug3ToolStripMenuItem->Text);
		 }
	private: System::Void sug4ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 replaceWord(sug4ToolStripMenuItem->Text);
		 }
	private: System::Void sug5ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 replaceWord(sug5ToolStripMenuItem->Text);
		 }
	private: System::Void ignoreToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 ignoreWord();
		 }
	private: System::Void addToDictionaryToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 addToDictionary();
		 }
	private: System::Void buttonImageFit_Click(System::Object^  sender, System::EventArgs^  e) {	 
			 this->pictureBoxShow->Dock = System::Windows::Forms::DockStyle::None;
			 
			 // Correct the image size based on aspect ratio
			 float aspRatioFactor;
			 if(this->im->Height > this->im->Width)
			 {
				 aspRatioFactor = (float)this->im->Height/scaleY;

				 this->pictureBoxShow->Height = scaleY;
				 this->pictureBoxShow->Width = this->im->Width / aspRatioFactor;
			 }else{
				 aspRatioFactor = (float)this->im->Width/scaleX;

				 this->pictureBoxShow->Height = this->im->Height / aspRatioFactor;
				 this->pictureBoxShow->Width = scaleX;
			 }

			 this->pictureBoxShow->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			 this->pictureBoxShow->Update();

			 // zoom factors
			 this->zoomHeight = this->pictureBoxShow->Height;
			 this->zoomWidth = this->pictureBoxShow->Width;
		 }
	private: System::Void buttonImageVerticallyFit_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->pictureBoxShow->Dock = System::Windows::Forms::DockStyle::None;

				 this->pictureBoxShow->Height = im->Height;
				 this->pictureBoxShow->Width  = scaleX;

				 this->pictureBoxShow->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;

				 this->pictureBoxShow->Update();

				 // zoom factors
				 this->zoomHeight = this->pictureBoxShow->Height;
				 this->zoomWidth = this->pictureBoxShow->Width;
		 }
	private: System::Void buttonImageHorizontallyFit_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->pictureBoxShow->Dock = System::Windows::Forms::DockStyle::None;

				 this->pictureBoxShow->Height = scaleY;
				 this->pictureBoxShow->Width  = im->Width;

				 this->pictureBoxShow->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;

				 this->pictureBoxShow->Update();

				 // zoom factors
				 this->zoomHeight = this->pictureBoxShow->Height;
				 this->zoomWidth = this->pictureBoxShow->Width;
		 }
	private: System::Void buttonZoomIn_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->pictureBoxShow->Dock = System::Windows::Forms::DockStyle::None;

				 // zoom factors
				 this->zoomHeight += this->zoomHeight * 0.1;
				 this->zoomWidth  += this->zoomWidth * 0.1;

				 this->pictureBoxShow->Height = this->zoomHeight;
				 this->pictureBoxShow->Width  = this->zoomWidth;

				 this->pictureBoxShow->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;

				 this->pictureBoxShow->Update();
		 }
	private: System::Void buttonZoomOut_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->pictureBoxShow->Dock = System::Windows::Forms::DockStyle::None;

			 // zoom factors
			 this->zoomHeight -= this->zoomHeight * 0.1;
			 this->zoomWidth  -= this->zoomWidth * 0.1;

			 this->pictureBoxShow->Height = this->zoomHeight;
			 this->pictureBoxShow->Width  = this->zoomWidth;

			 this->pictureBoxShow->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;

			 this->pictureBoxShow->Update();
		 }
	private: System::Void buttonImageOriginial_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->pictureBoxShow->Dock = System::Windows::Forms::DockStyle::None;
			 this->pictureBoxShow->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Normal;

			 this->pictureBoxShow->Height = im->Height;
			 this->pictureBoxShow->Width  = im->Width;

			 // zoom factors
			 this->zoomHeight = im->Height;
			 this->zoomWidth  = im->Width;
		 }
	private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 deleteTemporaryImageFilesUsingCMD();
		 }
	private: bool computeThreshold(){
			 try{		 
				 // compute the threshold value
				 ThresholedValue^ ts = gcnew ThresholedValue(im);
				 float fLevel = ts->GetThresholed();			 
				 thresholdValue = int(fLevel*255);
				 return true;
			 }
			 catch(System::Exception^ ex)
			 {
				 System::Windows::Forms::MessageBox::Show(ex->Message->ToString(),"Failed to compute threshold value!!",System::Windows::Forms::MessageBoxButtons::OK,System::Windows::Forms::MessageBoxIcon::Error);
				 return false;
			 }
}  
    private: void ComputeBinaryArray()
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
				grayC = grayImageArr[xVal][yVal];

				if ( grayC >= thresholdValue )
				{
					BArray[xVal][yVal] = true;
				}
				else
				{
					BArray[xVal][yVal] = false;
				}

			}// inner for*/
		}
	} 
	private: void getBainaryImage(){
				 BinaryImage = gcnew System::Drawing::Bitmap(ysize, xsize);
				 for (int x = 0; x < xsize; ++x)
				 {
					 for (int y = 0; y < ysize; ++y)
					 {
						 if(BArray[x][y]){
							 BinaryImage->SetPixel(y, x, Color::White);
						 }else{
							 BinaryImage->SetPixel(y, x, Color::Black);
						 }
					 }
				 }
			 }

	private: void saveBainaryImage(bool **tmpArray, int txs, int tys, int num){
				 System::Drawing::Bitmap^ tImage = gcnew System::Drawing::Bitmap(tys, txs);
				 for (int x = 0; x < txs; ++x)
				 {
					 for (int y = 0; y < tys; ++y)
					 {
						 if(tmpArray[x][y]){
							 tImage->SetPixel(y, x, Color::White);
						 }else{
							 tImage->SetPixel(y, x, Color::Black);
						 }
					 }
				 }

				 tImage->Save("C:\\"+num+".jpg");
			 }

};
}

