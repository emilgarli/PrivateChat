#pragma once
#include "ChatGUIForm.h"
#include "vector"
#include "thread"
#include <msclr/marshal_cppstd.h> 
#include "ConnectionHandler.h"
using namespace System;
using namespace System::Windows::Forms;

vector<ConnectionHandler*> activeConnections;


// Convert System::String^ to std::string
std::string GUICLR::ChatGUIForm::convertToStdString(System::String^ sysString) {
	if (sysString != nullptr && sysString != "")
		return msclr::interop::marshal_as<std::string>(sysString);
	else
		return "";
}

System::String^ ConvertStdStringToSystemString(const std::string& str) {
	return gcnew System::String(str.c_str());
}

void GUICLR::ChatGUIForm::writeMessageToChat(const char* message, int mesLen, string recip) {
	for (ConnectionHandler* connection : activeConnections) {
		if (connection->getClientName() == recip) {
			if (connection->writeToChat(message, mesLen) == mesLen) {
				logMessage(connection->getName() + ": " + message);
			}
		}
	}
}

void GUICLR::ChatGUIForm::UpdateConnectionList() {
	// Check if we need to invoke to the UI thread
	if (this->listView1->InvokeRequired) {
		// Create a delegate to call UpdateConnectionList
		this->listView1->Invoke(gcnew Action(this, &ChatGUIForm::UpdateConnectionList));
		return;
	}

	// Clear existing items in the ListView
	this->listView1->Items->Clear();
	//Lock the mutex until client name has been read from the connection
	//Condition variable is used to notify

	for (ConnectionHandler* handler : activeConnections) {
		// Convert std::string from getName() to System::String^
		std::string connectionName = handler->getClientName();
		System::String^ managedName = gcnew System::String(connectionName.c_str());

		// Create a new ListViewItem and add it to the ListView
		ListViewItem^ item = gcnew ListViewItem(managedName);
		this->listView1->Items->Add(item);
	}
}


int GUICLR::ChatGUIForm::connectToPeer(const char* ipAddress, int portNum) {

	CWizReadWriteSocket* socket = new CWizReadWriteSocket();
	string peerName = ipAddress;

	if (!socket->Connect(ipAddress, portNum)) {
		this->logMessage("Connect failed");
	}
	if (WSAGetLastError() != 0) {
		this->logMessage("Error:" + GetLastError());
		return -1;
	}
	//std::shared_ptr<ConnectionHandler> handler = std::make_shared<ConnectionHandler>(socket, convertToStdString(name));
	ConnectionHandler* handler = new ConnectionHandler(socket, convertToStdString(name));
	handler->setManagedObject(this);
	std::thread handlerThread(&ConnectionHandler::connectionRunner, handler);
	handlerThread.detach();
	activeConnections.push_back(handler);

	char inBuf[20]{};
	int iRead = 0;
	while (iRead == 0) {
		socket->Read(inBuf, 20, 1000);
	}

	handler->setClientName((string)inBuf);
	UpdateConnectionList();
	return 0;
}
void GUICLR::ChatGUIForm::listenThread() {
	//Wait a little while so the controls can be created
	Sleep(500);
	//Initialize winsock
	int iResult;
	WSADATA wsaData;
	int portNumber = PRIMARY_PORT;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}
	//Create listening socket
	CWizSyncSocket* serversocket = new CWizSyncSocket(PRIMARY_PORT, SOCK_STREAM);

	if (WSAGetLastError() != 0) {
		this->logMessage("Unable to bind socket to port " + PRIMARY_PORT);
		logMessage("Attempting backup listenport " + SECONDARY_PORT);
		CWizSyncSocket* serversocket = new CWizSyncSocket(SECONDARY_PORT, SOCK_STREAM);
		portNumber = SECONDARY_PORT;
	}
	if (WSAGetLastError() != 0) {
		logMessage("Failed to bind both ports. Aborting...");
		portNumber = 0;
		return;
	}
	logMessage("Listening for incomming connections on port " + portNumber);
	while (true) {
		SOCKET sock = serversocket->Accept();
		if (sock == INVALID_SOCKET) {
			logMessage("Failed to accept incomming connection.");
			continue;
		}
		CWizReadWriteSocket* socket = new CWizReadWriteSocket;

		if (WSAGetLastError() != 0) {
			logMessage("Failed to create new socket");
			continue;
		}

		socket->SetSocket(sock);
		logMessage("Incomming connection accepted");
		ConnectionHandler* handler = new ConnectionHandler(socket, convertToStdString(name));
		handler->setManagedObject(this);
		std::thread handlerThread(&ConnectionHandler::connectionRunner, handler);
		handlerThread.detach();
		activeConnections.push_back(handler);

		char inBuf[20]{};
		int iRead = 0;
		while (iRead == 0) {
			//Why does recv work, but not Read????
			iRead = socket->Read(inBuf, 20, 1000);
			iRead = recv(socket->H(), inBuf, 20, 0);
		}

		handler->setClientName((string)inBuf);
		UpdateConnectionList();
	}
}

void GUICLR::ChatGUIForm::logMessage(System::String^ message)
{
	// Check if Invoke is required (i.e., if the call is coming from a different thread)
	if (this->LogWindow->InvokeRequired)
	{
		// Create a delegate to point to this function and invoke it on the UI thread
		this->Invoke(gcnew Action<System::String^>(this, &GUICLR::ChatGUIForm::logMessage), message);
	}
	else
	{
		// If we're already on the UI thread, append the message directly
		this->LogWindow->AppendText(message + Environment::NewLine);
	}
}
void GUICLR::ChatGUIForm::logMessage(std::string stdmessage)
{
	System::String^ message = ConvertStdStringToSystemString(stdmessage);
	// Check if Invoke is required (i.e., if the call is coming from a different thread)
	if (this->LogWindow->InvokeRequired)
	{
		// Create a delegate to point to this function and invoke it on the UI thread
		this->Invoke(gcnew Action<System::String^>(this, &GUICLR::ChatGUIForm::logMessage), message);
	}
	else
	{
		// If we're already on the UI thread, append the message directly
		this->LogWindow->AppendText(message + Environment::NewLine);
	}
}

static vector<string> delimitString(const char* buffer, int bufLen, char delimit) {
	vector<string> vRet; // Use vector<string> to store actual strings
	string tempChar = "";

	for (int i = 0; i < bufLen; i++) {
		if (buffer[i] != delimit) {
			tempChar += buffer[i]; // Append character to tempChar
		}
		else {
			if (!tempChar.empty()) {
				vRet.push_back(tempChar); // Store the current string in the vector
				tempChar = ""; // Reset tempChar for the next substring
			}
		}
	}

	// Push the last substring (if any) after the loop
	if (!tempChar.empty()) {
		vRet.push_back(tempChar);
	}

	return vRet; // Return the vector of substrings
}

int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	GUICLR::ChatGUIForm form;
	Application::Run(% form);
}
