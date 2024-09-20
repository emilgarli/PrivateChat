#include "ConnectionHandler.h"
#include "vector"
#include "thread"

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

ConnectionHandler::ConnectionHandler(CWizReadWriteSocket* sock,  string name)
{
	socket = sock;
	this->name = name;
}

int ConnectionHandler::writeToChat(const char* message, int messageLength)
{
	//Write returns the number of bytes successfully written to the connection
	return socket->Write(message, messageLength);
}

int ConnectionHandler::readFromChat(char* buffer, int bufLen)
{
	while (true) {
		int iRead = 0;
		while (iRead == 0) {
			//iRead = recv(socket->H(), buffer, bufLen, 0);
			iRead = socket->Read(buffer, bufLen, 100);
			Sleep(50);
		}
	}
	return 0;
}

int ConnectionHandler::readHandler() {
	char inBuffer[MAX_MESSAGE_SIZE]{};
	while (true) {
		int iRead = 0;
		iRead = readFromChat(inBuffer, MAX_MESSAGE_SIZE);
		if (iRead > 0) {
			System::String^ message = gcnew System::String(inBuffer);
			form->logMessage(message);
		}
	}
}

int ConnectionHandler::connectionRunner()
{
	thread readThread(&ConnectionHandler::readHandler, this);
	readThread.detach();  // Make sure the thread runs independently

	return 0;
}



