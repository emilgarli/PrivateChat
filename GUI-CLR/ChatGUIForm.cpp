#include "ChatGUIForm.h"
#include "ConnectionHandler.h"
#include "vector"
#include "thread"
#include <msclr/marshal_cppstd.h> 
using namespace System;
using namespace System::Windows::Forms;

vector<std::shared_ptr<ConnectionHandler>> activeConnections;

// Convert System::String^ to std::string
std::string convertToStdString(System::String^ sysString) {
	return msclr::interop::marshal_as<std::string>(sysString);
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
	if (AllocConsole()) {
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
	}
	else {
		this->logMessage("Failed to allocate console");
		return -1;
	}
	std::shared_ptr<ConnectionHandler> handler = std::make_shared<ConnectionHandler>(socket, convertToStdString(name));
	std::thread handlerThread(&ConnectionHandler::connectionRunner, handler);
	handlerThread.detach();
	activeConnections.push_back(handler);
	return 0;
}
void GUICLR::ChatGUIForm::listenThread() {
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
		auto socket = new CWizReadWriteSocket;

		if (WSAGetLastError() != 0) {
			logMessage("Failed to create new socket");
			continue;
		}

		socket->SetSocket(sock);
		logMessage("Incomming connection!");
		
		std::shared_ptr<ConnectionHandler> handler = std::make_shared<ConnectionHandler>(socket, convertToStdString(name));
		std::thread handlerThread(&ConnectionHandler::connectionRunner, handler);
		handlerThread.detach();
		activeConnections.push_back(handler);
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
