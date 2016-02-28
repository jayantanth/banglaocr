#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;


namespace BanglaOCR {

	/// <summary>
	/// Summary for FormAbout
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormAbout : public System::Windows::Forms::Form
	{
	public:
		FormAbout(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormAbout()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Panel^  panelAboutName;
	private: System::Windows::Forms::Panel^  panelBody;


	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Panel^  panelAbout;
	private: System::Windows::Forms::Panel^  panelButton;
	private: System::Windows::Forms::Button^  buttonCredits;



	private: System::Windows::Forms::Panel^  panelCredits;
	private: System::Windows::Forms::TextBox^  textBoxCredits;


	private: System::Windows::Forms::Button^  button1;




	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormAbout::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->panelAboutName = (gcnew System::Windows::Forms::Panel());
			this->panelBody = (gcnew System::Windows::Forms::Panel());
			this->panelAbout = (gcnew System::Windows::Forms::Panel());
			this->panelCredits = (gcnew System::Windows::Forms::Panel());
			this->textBoxCredits = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->panelButton = (gcnew System::Windows::Forms::Panel());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->buttonCredits = (gcnew System::Windows::Forms::Button());
			this->panelAboutName->SuspendLayout();
			this->panelBody->SuspendLayout();
			this->panelAbout->SuspendLayout();
			this->panelCredits->SuspendLayout();
			this->panelButton->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::Color::Red;
			this->label1->Location = System::Drawing::Point(83, 5);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(157, 58);
			this->label1->TabIndex = 1;
			this->label1->Text = L"BanglaOCR \n ";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::Transparent;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(161, 31);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(79, 15);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Version 0.7";
			// 
			// panelAboutName
			// 
			this->panelAboutName->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"panelAboutName.BackgroundImage")));
			this->panelAboutName->Controls->Add(this->label2);
			this->panelAboutName->Controls->Add(this->label1);
			this->panelAboutName->Dock = System::Windows::Forms::DockStyle::Top;
			this->panelAboutName->Location = System::Drawing::Point(0, 0);
			this->panelAboutName->Name = L"panelAboutName";
			this->panelAboutName->Size = System::Drawing::Size(325, 63);
			this->panelAboutName->TabIndex = 2;
			// 
			// panelBody
			// 
			this->panelBody->Controls->Add(this->panelAbout);
			this->panelBody->Controls->Add(this->panelButton);
			this->panelBody->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelBody->Location = System::Drawing::Point(0, 63);
			this->panelBody->Name = L"panelBody";
			this->panelBody->Size = System::Drawing::Size(325, 123);
			this->panelBody->TabIndex = 3;
			// 
			// panelAbout
			// 
			this->panelAbout->Controls->Add(this->panelCredits);
			this->panelAbout->Controls->Add(this->label3);
			this->panelAbout->Controls->Add(this->label4);
			this->panelAbout->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelAbout->Location = System::Drawing::Point(0, 0);
			this->panelAbout->Name = L"panelAbout";
			this->panelAbout->Size = System::Drawing::Size(325, 78);
			this->panelAbout->TabIndex = 5;
			// 
			// panelCredits
			// 
			this->panelCredits->Controls->Add(this->textBoxCredits);
			this->panelCredits->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelCredits->Location = System::Drawing::Point(0, 0);
			this->panelCredits->Name = L"panelCredits";
			this->panelCredits->Size = System::Drawing::Size(325, 78);
			this->panelCredits->TabIndex = 3;
			this->panelCredits->Visible = false;
			// 
			// textBoxCredits
			// 
			this->textBoxCredits->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBoxCredits->Location = System::Drawing::Point(25, 6);
			this->textBoxCredits->Multiline = true;
			this->textBoxCredits->Name = L"textBoxCredits";
			this->textBoxCredits->ReadOnly = true;
			this->textBoxCredits->Size = System::Drawing::Size(277, 50);
			this->textBoxCredits->TabIndex = 0;
			this->textBoxCredits->Text = L"Md. Abul Hasnat, S. M. Murtoza Habib and Mumit Khan\r\nCenter For Research on Bangl" 
				L"a Language Processing\r\nBRAC University";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(22, 15);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(177, 13);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Copyright (c) 2006 BRAC University.";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(22, 43);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(181, 13);
			this->label4->TabIndex = 2;
			this->label4->Text = L"Released under GNU Public License";
			// 
			// panelButton
			// 
			this->panelButton->Controls->Add(this->button1);
			this->panelButton->Controls->Add(this->buttonCredits);
			this->panelButton->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->panelButton->Location = System::Drawing::Point(0, 78);
			this->panelButton->Name = L"panelButton";
			this->panelButton->Size = System::Drawing::Size(325, 45);
			this->panelButton->TabIndex = 4;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(109, 9);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(104, 26);
			this->button1->TabIndex = 4;
			this->button1->Text = L"< About";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &FormAbout::button1_Click);
			// 
			// buttonCredits
			// 
			this->buttonCredits->Location = System::Drawing::Point(3, 9);
			this->buttonCredits->Name = L"buttonCredits";
			this->buttonCredits->Size = System::Drawing::Size(104, 26);
			this->buttonCredits->TabIndex = 3;
			this->buttonCredits->Text = L"Credits";
			this->buttonCredits->UseVisualStyleBackColor = true;
			this->buttonCredits->Click += gcnew System::EventHandler(this, &FormAbout::buttonCredits_Click);
			// 
			// FormAbout
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(325, 186);
			this->Controls->Add(this->panelBody);
			this->Controls->Add(this->panelAboutName);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"FormAbout";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"About BanglaOCR";
			this->panelAboutName->ResumeLayout(false);
			this->panelAboutName->PerformLayout();
			this->panelBody->ResumeLayout(false);
			this->panelAbout->ResumeLayout(false);
			this->panelAbout->PerformLayout();
			this->panelCredits->ResumeLayout(false);
			this->panelCredits->PerformLayout();
			this->panelButton->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void buttonCredits_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->panelCredits->Visible = true;
			 }
private: System::Void buttonAcknowledgement_Click(System::Object^  sender, System::EventArgs^  e) {

			 this->panelCredits->Visible = false;
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 this->panelCredits->Visible = false;
		 }
};
}
