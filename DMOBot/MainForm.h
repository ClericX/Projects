#pragma once

namespace DMOBot {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
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
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::CheckBox^  AutoChatCheckBox;
	private: System::Windows::Forms::CheckBox^  AutoPotionCheckBox;
	private: System::Windows::Forms::CheckBox^  FMStealerCheckBox;
	protected: 


	private: System::Windows::Forms::Button^  AutoChatLoadButton;
	private: System::Windows::Forms::Button^  AutoPotSetHPButton;


	private: System::Windows::Forms::Button^  FMStealerSetButton;
	private: System::Windows::Forms::Button^  AutoPotSetMPButton;
	private: System::Windows::Forms::TextBox^  AutoChatDelayTextBox;


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
			this->AutoChatCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->AutoPotionCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->FMStealerCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->AutoChatLoadButton = (gcnew System::Windows::Forms::Button());
			this->AutoPotSetHPButton = (gcnew System::Windows::Forms::Button());
			this->FMStealerSetButton = (gcnew System::Windows::Forms::Button());
			this->AutoPotSetMPButton = (gcnew System::Windows::Forms::Button());
			this->AutoChatDelayTextBox = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// AutoChatCheckBox
			// 
			this->AutoChatCheckBox->AutoCheck = false;
			this->AutoChatCheckBox->AutoSize = true;
			this->AutoChatCheckBox->Location = System::Drawing::Point(12, 12);
			this->AutoChatCheckBox->Name = L"AutoChatCheckBox";
			this->AutoChatCheckBox->Size = System::Drawing::Size(73, 17);
			this->AutoChatCheckBox->TabIndex = 0;
			this->AutoChatCheckBox->Text = L"Auto Chat";
			this->AutoChatCheckBox->UseVisualStyleBackColor = true;
			this->AutoChatCheckBox->Click += gcnew System::EventHandler(this, &MainForm::AutoChatCheckBox_Click);
			// 
			// AutoPotionCheckBox
			// 
			this->AutoPotionCheckBox->AutoCheck = false;
			this->AutoPotionCheckBox->AutoSize = true;
			this->AutoPotionCheckBox->Location = System::Drawing::Point(12, 41);
			this->AutoPotionCheckBox->Name = L"AutoPotionCheckBox";
			this->AutoPotionCheckBox->Size = System::Drawing::Size(81, 17);
			this->AutoPotionCheckBox->TabIndex = 1;
			this->AutoPotionCheckBox->Text = L"Auto Potion";
			this->AutoPotionCheckBox->UseVisualStyleBackColor = true;
			this->AutoPotionCheckBox->Click += gcnew System::EventHandler(this, &MainForm::AutoPotionCheckBox_Click);
			// 
			// FMStealerCheckBox
			// 
			this->FMStealerCheckBox->AutoCheck = false;
			this->FMStealerCheckBox->AutoSize = true;
			this->FMStealerCheckBox->Location = System::Drawing::Point(12, 70);
			this->FMStealerCheckBox->Name = L"FMStealerCheckBox";
			this->FMStealerCheckBox->Size = System::Drawing::Size(77, 17);
			this->FMStealerCheckBox->TabIndex = 2;
			this->FMStealerCheckBox->Text = L"FM Stealer";
			this->FMStealerCheckBox->UseVisualStyleBackColor = true;
			this->FMStealerCheckBox->Click += gcnew System::EventHandler(this, &MainForm::FMStealerCheckBox_Click);
			// 
			// AutoChatLoadButton
			// 
			this->AutoChatLoadButton->Location = System::Drawing::Point(103, 8);
			this->AutoChatLoadButton->Name = L"AutoChatLoadButton";
			this->AutoChatLoadButton->Size = System::Drawing::Size(75, 23);
			this->AutoChatLoadButton->TabIndex = 3;
			this->AutoChatLoadButton->Text = L"Load";
			this->AutoChatLoadButton->UseVisualStyleBackColor = true;
			this->AutoChatLoadButton->Click += gcnew System::EventHandler(this, &MainForm::AutoChatLoadButton_Click);
			// 
			// AutoPotSetHPButton
			// 
			this->AutoPotSetHPButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->AutoPotSetHPButton->Location = System::Drawing::Point(103, 37);
			this->AutoPotSetHPButton->Name = L"AutoPotSetHPButton";
			this->AutoPotSetHPButton->Size = System::Drawing::Size(75, 23);
			this->AutoPotSetHPButton->TabIndex = 4;
			this->AutoPotSetHPButton->Text = L"Set HP";
			this->AutoPotSetHPButton->UseVisualStyleBackColor = true;
			this->AutoPotSetHPButton->Click += gcnew System::EventHandler(this, &MainForm::AutoPotSetHPButton_Click);
			// 
			// FMStealerSetButton
			// 
			this->FMStealerSetButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->FMStealerSetButton->Location = System::Drawing::Point(103, 66);
			this->FMStealerSetButton->Name = L"FMStealerSetButton";
			this->FMStealerSetButton->Size = System::Drawing::Size(156, 23);
			this->FMStealerSetButton->TabIndex = 5;
			this->FMStealerSetButton->Text = L"Set";
			this->FMStealerSetButton->UseVisualStyleBackColor = true;
			this->FMStealerSetButton->Click += gcnew System::EventHandler(this, &MainForm::FMStealerSetButton_Click);
			// 
			// AutoPotSetMPButton
			// 
			this->AutoPotSetMPButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->AutoPotSetMPButton->Location = System::Drawing::Point(184, 37);
			this->AutoPotSetMPButton->Name = L"AutoPotSetMPButton";
			this->AutoPotSetMPButton->Size = System::Drawing::Size(75, 23);
			this->AutoPotSetMPButton->TabIndex = 6;
			this->AutoPotSetMPButton->Text = L"Set MP";
			this->AutoPotSetMPButton->UseVisualStyleBackColor = true;
			this->AutoPotSetMPButton->Click += gcnew System::EventHandler(this, &MainForm::AutoPotSetMPButton_Click);
			// 
			// AutoChatDelayTextBox
			// 
			this->AutoChatDelayTextBox->Location = System::Drawing::Point(184, 10);
			this->AutoChatDelayTextBox->Name = L"AutoChatDelayTextBox";
			this->AutoChatDelayTextBox->Size = System::Drawing::Size(75, 20);
			this->AutoChatDelayTextBox->TabIndex = 7;
			this->AutoChatDelayTextBox->Text = L"1000";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(272, 103);
			this->Controls->Add(this->AutoChatDelayTextBox);
			this->Controls->Add(this->AutoPotSetMPButton);
			this->Controls->Add(this->FMStealerSetButton);
			this->Controls->Add(this->AutoPotSetHPButton);
			this->Controls->Add(this->AutoChatLoadButton);
			this->Controls->Add(this->FMStealerCheckBox);
			this->Controls->Add(this->AutoPotionCheckBox);
			this->Controls->Add(this->AutoChatCheckBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"MS Bot";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private:
	void AutoChatCheckBox_Click(System::Object^  sender, System::EventArgs^  e);
	void AutoPotionCheckBox_Click(System::Object^  sender, System::EventArgs^  e);
	void FMStealerCheckBox_Click(System::Object^  sender, System::EventArgs^  e);
	void AutoChatLoadButton_Click(System::Object^  sender, System::EventArgs^  e);
	void AutoPotSetHPButton_Click(System::Object^  sender, System::EventArgs^  e);
	void AutoPotSetMPButton_Click(System::Object^  sender, System::EventArgs^  e);
	void FMStealerSetButton_Click(System::Object^  sender, System::EventArgs^  e);
};
}