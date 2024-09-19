#include "ConnectionHandler.h"
#include "vector"

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
}

int ConnectionHandler::writeToChat(const char* message, int messageLength)
{
	//Write returns the number of bytes successfully written to the connection
	return socket->Write(message, messageLength);
}

int ConnectionHandler::readFromChat(char* buffer, int bufLen)
{
	int iRead = 0;
	while (iRead == 0) {
		iRead = socket->Read(buffer, bufLen, 1000);
	}
	return iRead;
}

int ConnectionHandler::connectionRunner()
{
	char infoBuffer[MAX_MESSAGE_SIZE]{};
	int iRead = readFromChat(infoBuffer, MAX_MESSAGE_SIZE);
	vector<string> infoVec = delimitString(infoBuffer, MAX_MESSAGE_SIZE, ',');
	clientName = infoVec.at(0);
	return 0;
}

