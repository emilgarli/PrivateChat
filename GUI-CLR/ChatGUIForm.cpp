#include "ChatGUIForm.h"
#include <Rawsocket.h>

using namespace System::Threading;

using namespace System;
using namespace System::Windows::Forms;

#define MAX_MESSAGE_SIZE 200

// Modify functions to accept a reference to ChatGUIForm
static void writeChat(CWizReadWriteSocket* socket, GUICLR::ChatGUIForm^ form) {
	while (true) {
		string output = "";
		if (output.length() > MAX_MESSAGE_SIZE) {
			return;
		}
		socket->Write(output.c_str(), output.length());
		if (WSAGetLastError() != 0) {
			// Log an error message
			form->Invoke(gcnew Action<System::String^>(form, &GUICLR::ChatGUIForm::logMessage), "Unable to write: " + GetLastError());
		}
	}
}

static void readChat(CWizReadWriteSocket* socket, GUICLR::ChatGUIForm^ form)
{
	while (true) {
		char inBuf[MAX_MESSAGE_SIZE]{};
		int iRead = 0;
		while (iRead < 1) {
			iRead = socket->Read(inBuf, MAX_MESSAGE_SIZE, 1000);
			Sleep(10);
		}
		// Log the received message
		form->Invoke(gcnew Action<System::String^>(form, &GUICLR::ChatGUIForm::logMessage), "Message received: " + gcnew String(inBuf));
	}
}



int GUICLR::ChatGUIForm::connectToPeer(const char* ipAddress, int portNum, GUICLR::ChatGUIForm^ form) {

	form->Invoke(gcnew Action<System::String^>(form, &GUICLR::ChatGUIForm::logMessage), "Suq Madiq is my name-o!");
	CWizReadWriteSocket* socket = new CWizReadWriteSocket();
	string peerName = ipAddress;

	if (!socket->Connect(ipAddress, portNum)) {
		form->Invoke(gcnew Action<System::String^>(form, &GUICLR::ChatGUIForm::logMessage), "Connection failed");
	}
	if (WSAGetLastError() != 0) {
		form->Invoke(gcnew Action<System::String^>(form, &GUICLR::ChatGUIForm::logMessage), "WSAGetLastError error");
		return -1;
	}
	if (AllocConsole()) {
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
	}
	else {
		form->Invoke(gcnew Action<System::String^>(form, &GUICLR::ChatGUIForm::logMessage), "Failed to allocate console");
		return -1;
	}
	// Start the receive thread using a lambda function to pass arguments
	Thread^ recThread = gcnew Thread(gcnew ParameterizedThreadStart(&readChat, socket, form));

	return 0;
}

// Modify logMessage to accept a string parameter
void GUICLR::ChatGUIForm::logMessage(System::String^ message)
{
	this->LogWindow->AppendText(message + Environment::NewLine);
}

int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	GUICLR::ChatGUIForm form;
	Application::Run(% form);
}
