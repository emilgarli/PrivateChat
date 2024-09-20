#pragma once

#include "Rawsocket.h"
#include "mutex"


#define MAX_MESSAGE_SIZE 200
#define PRIMARY_PORT 17590
#define SECONDARY_PORT 17591

namespace GUICLR {
	using namespace System::Threading;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace System::Collections::Generic;
	/// <summary>
	/// Summary for ChatGUIForm
	/// </summary>
	public ref class ChatGUIForm : public System::Windows::Forms::Form
	{
	public:
		ChatGUIForm(void)
		{
			InitializeComponent();
			Thread^ listenThread = gcnew Thread(gcnew ThreadStart(this, &GUICLR::ChatGUIForm::listenThread));
			listenThread->Start();
			//
			//TODO: Add the constructor code here
			//

		}
		static std::string convertToStdString(System::String^ sysString);
		void writeMessageToChat(const char* message, int mesLen, string recip);
		void UpdateConnectionList();
		int connectToPeer(const char* ipAddress, int portNum);
		void listenThread();
		void logMessage(System::String^ message);

		void logMessage(std::string message);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ChatGUIForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ LogWindow;
	private: System::Windows::Forms::TextBox^ IpBox;
	private: System::Windows::Forms::TextBox^ PortBox;
	private: String^ name;
	private: String^ currentClient;
	private: System::Windows::Forms::Button^ SetIP;
	private: System::Windows::Forms::Button^ bSetPort;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: String^ ipAddress;
	private: int portNumber;
	private: System::Windows::Forms::TextBox^ sendtextbox;
	private: System::Windows::Forms::ListView^ listView1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ YourName;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ bSetName;

	private: System::Windows::Forms::Button^ Sendbutton;
		   

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->LogWindow = (gcnew System::Windows::Forms::TextBox());
			this->IpBox = (gcnew System::Windows::Forms::TextBox());
			this->PortBox = (gcnew System::Windows::Forms::TextBox());
			this->SetIP = (gcnew System::Windows::Forms::Button());
			this->bSetPort = (gcnew System::Windows::Forms::Button());
			this->sendtextbox = (gcnew System::Windows::Forms::TextBox());
			this->Sendbutton = (gcnew System::Windows::Forms::Button());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->YourName = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->bSetName = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// LogWindow
			// 
			this->LogWindow->Location = System::Drawing::Point(12, 244);
			this->LogWindow->Multiline = true;
			this->LogWindow->Name = L"LogWindow";
			this->LogWindow->Size = System::Drawing::Size(1175, 238);
			this->LogWindow->TabIndex = 0;
			this->LogWindow->TextChanged += gcnew System::EventHandler(this, &ChatGUIForm::textBox1_TextChanged);
			// 
			// IpBox
			// 
			this->IpBox->Location = System::Drawing::Point(12, 12);
			this->IpBox->Name = L"IpBox";
			this->IpBox->Size = System::Drawing::Size(162, 20);
			this->IpBox->TabIndex = 1;
			this->IpBox->Text = L"127.0.0.1";
			this->IpBox->TextChanged += gcnew System::EventHandler(this, &ChatGUIForm::IpBox_TextChanged);
			// 
			// PortBox
			// 
			this->PortBox->Location = System::Drawing::Point(261, 12);
			this->PortBox->Name = L"PortBox";
			this->PortBox->Size = System::Drawing::Size(119, 20);
			this->PortBox->TabIndex = 2;
			this->PortBox->Text = L"17591";
			this->PortBox->TextChanged += gcnew System::EventHandler(this, &ChatGUIForm::textBox2_TextChanged);
			// 
			// SetIP
			// 
			this->SetIP->Location = System::Drawing::Point(180, 9);
			this->SetIP->Name = L"SetIP";
			this->SetIP->Size = System::Drawing::Size(75, 23);
			this->SetIP->TabIndex = 3;
			this->SetIP->Text = L"Set IP";
			this->SetIP->UseVisualStyleBackColor = true;
			this->SetIP->Click += gcnew System::EventHandler(this, &ChatGUIForm::button1_Click);
			// 
			// bSetPort
			// 
			this->bSetPort->Location = System::Drawing::Point(387, 8);
			this->bSetPort->Name = L"bSetPort";
			this->bSetPort->Size = System::Drawing::Size(75, 23);
			this->bSetPort->TabIndex = 4;
			this->bSetPort->Text = L"Set Port";
			this->bSetPort->UseVisualStyleBackColor = true;
			this->bSetPort->Click += gcnew System::EventHandler(this, &ChatGUIForm::button2_Click);
			// 
			// sendtextbox
			// 
			this->sendtextbox->Location = System::Drawing::Point(12, 488);
			this->sendtextbox->Multiline = true;
			this->sendtextbox->Name = L"sendtextbox";
			this->sendtextbox->Size = System::Drawing::Size(450, 62);
			this->sendtextbox->TabIndex = 5;
			this->sendtextbox->TextChanged += gcnew System::EventHandler(this, &ChatGUIForm::InputBox_TextChanged);
			// 
			// Sendbutton
			// 
			this->Sendbutton->Location = System::Drawing::Point(469, 489);
			this->Sendbutton->Name = L"Sendbutton";
			this->Sendbutton->Size = System::Drawing::Size(81, 61);
			this->Sendbutton->TabIndex = 6;
			this->Sendbutton->Text = L"Send";
			this->Sendbutton->UseVisualStyleBackColor = true;
			this->Sendbutton->Click += gcnew System::EventHandler(this, &ChatGUIForm::Sendbutton_Click);
			// 
			// listView1
			// 
			this->listView1->HideSelection = false;
			this->listView1->Location = System::Drawing::Point(12, 115);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(439, 123);
			this->listView1->TabIndex = 7;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->SelectedIndexChanged += gcnew System::EventHandler(this, &ChatGUIForm::listView1_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 96);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(93, 13);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Connected Clients";
			// 
			// YourName
			// 
			this->YourName->Location = System::Drawing::Point(845, 11);
			this->YourName->Name = L"YourName";
			this->YourName->Size = System::Drawing::Size(100, 20);
			this->YourName->TabIndex = 9;
			this->YourName->TextChanged += gcnew System::EventHandler(this, &ChatGUIForm::YourName_TextChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(779, 12);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(60, 13);
			this->label2->TabIndex = 10;
			this->label2->Text = L"Your Name";
			// 
			// bSetName
			// 
			this->bSetName->Location = System::Drawing::Point(952, 8);
			this->bSetName->Name = L"bSetName";
			this->bSetName->Size = System::Drawing::Size(75, 23);
			this->bSetName->TabIndex = 11;
			this->bSetName->Text = L"Set Name";
			this->bSetName->UseVisualStyleBackColor = true;
			this->bSetName->Click += gcnew System::EventHandler(this, &ChatGUIForm::bSetName_Click);
			// 
			// ChatGUIForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->ClientSize = System::Drawing::Size(1199, 562);
			this->Controls->Add(this->bSetName);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->YourName);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->listView1);
			this->Controls->Add(this->Sendbutton);
			this->Controls->Add(this->sendtextbox);
			this->Controls->Add(this->bSetPort);
			this->Controls->Add(this->SetIP);
			this->Controls->Add(this->PortBox);
			this->Controls->Add(this->IpBox);
			this->Controls->Add(this->LogWindow);
			this->Name = L"ChatGUIForm";
			this->Text = L"ChatGUIForm";
			this->Load += gcnew System::EventHandler(this, &ChatGUIForm::ChatGUIForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {

	}
	private: System::Void IpBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {

	}
	private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e) {

	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		ipAddress = this->IpBox->Text;
		this->LogWindow->AppendText("IP address set to \"" + ipAddress + "\"" +  Environment::NewLine);
	}
	private: System::Void ChatGUIForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		try {
			portNumber = Int32::Parse(this->PortBox->Text);
		}
		catch (FormatException^ ex) {
			MessageBox::Show("Invalid port number. Please enter a valid number.");
		}
		this->LogWindow->AppendText("Port number set to " + portNumber + Environment::NewLine);
		const char* unmanagedString = (const char*)Marshal::StringToHGlobalAnsi(ipAddress).ToPointer();
		connectToPeer(unmanagedString, portNumber);
	}
	private: System::Void InputBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void Sendbutton_Click(System::Object^ sender, System::EventArgs^ e) {
		const char* unmanagedString = (const char*)Marshal::StringToHGlobalAnsi(this->sendtextbox->Text).ToPointer();
		this->sendtextbox->Text = "";
		writeMessageToChat(unmanagedString, sizeof(unmanagedString), convertToStdString(currentClient));
	}
	private: System::Void listView1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		if (this->listView1->SelectedItems->Count > 0) {
			// Get the currently selected item
			ListViewItem^ selectedItem = this->listView1->SelectedItems[0];

			// Set currentClient to the text of the selected item
			this->currentClient = selectedItem->Text;
		}
		else {
			// Optionally handle the case where no item is selected
			this->currentClient = ""; // or some default value
		}
	}
	private: System::Void YourName_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void bSetName_Click(System::Object^ sender, System::EventArgs^ e) {
		name = this->YourName->Text;
		this->LogWindow->AppendText("Your username has been set to: " + name + Environment::NewLine);
	}
}; }

