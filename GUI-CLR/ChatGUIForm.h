#pragma once
//#include <msclr/marshal.h>
//#include <msclr/marshal_cppstd.h>


namespace GUICLR {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for ChatGUIForm
	/// </summary>
	public ref class ChatGUIForm : public System::Windows::Forms::Form
	{
	public:
		ChatGUIForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

		}
		static int connectToPeer(const char* ipAddress, int portNum, GUICLR::ChatGUIForm^ form);
		void logMessage(System::String^ message);

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

	private: System::Windows::Forms::Button^ SetIP;

	private: System::Windows::Forms::Button^ SetPort;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		String^ ipAddress;
		int portNumber;

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
			this->SetPort = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// LogWindow
			// 
			this->LogWindow->Location = System::Drawing::Point(12, 312);
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
			this->IpBox->Text = L"Enter IP";
			this->IpBox->TextChanged += gcnew System::EventHandler(this, &ChatGUIForm::IpBox_TextChanged);
			// 
			// PortBox
			// 
			this->PortBox->Location = System::Drawing::Point(261, 12);
			this->PortBox->Name = L"PortBox";
			this->PortBox->Size = System::Drawing::Size(119, 20);
			this->PortBox->TabIndex = 2;
			this->PortBox->Text = L"Enter Port";
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
			// SetPort
			// 
			this->SetPort->Location = System::Drawing::Point(387, 8);
			this->SetPort->Name = L"SetPort";
			this->SetPort->Size = System::Drawing::Size(75, 23);
			this->SetPort->TabIndex = 4;
			this->SetPort->Text = L"Set Port";
			this->SetPort->UseVisualStyleBackColor = true;
			this->SetPort->Click += gcnew System::EventHandler(this, &ChatGUIForm::button2_Click);
			// 
			// ChatGUIForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1199, 562);
			this->Controls->Add(this->SetPort);
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
		this->LogWindow->AppendText("IP address set to " + ipAddress + Environment::NewLine);
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
		connectToPeer(unmanagedString, portNumber, this);
	}
	}; }

