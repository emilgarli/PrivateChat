#pragma once
#include "ChatGUIForm.h"
#include "vcclr.h"

class ConnectionHandler {
public:
	//Constructor
	ConnectionHandler(CWizReadWriteSocket* sock, string name);
	//Writes the given message to the socket connection
	int writeToChat(const char* message, int messageLength);
	//Reads the socket and returns the message
	int readFromChat(char* buffer, int bufLen);

	int readHandler();

	string getClientName() { return clientName; }
	string getName() { return name; }

	void setManagedObject(GUICLR::ChatGUIForm^ managedObj) { form = managedObj; }
	void setClientName(string newClientName) { clientName = newClientName; }

	int connectionRunner();

private:
	gcroot<GUICLR::ChatGUIForm^> form;
	CWizReadWriteSocket* socket = nullptr;
	string name = "";
	string clientName = "Default name";
};