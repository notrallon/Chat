#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "User.h"

int main() 
{
	// ----- The server -----
	// Create a socket and bind it to the port 55002
	sf::UdpSocket socket;
	socket.bind(55002);

	std::vector<User*> connectedUsers;

	while (true) {
		// Receive a message from anyone
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);

		std::string bufferStr = buffer;

		User* sendingUser = nullptr;

		if (connectedUsers.size() > 0) 
		{
			for (int i = 0; i < connectedUsers.size(); i++)
			{
				if (connectedUsers[i]->GetAdress() == sender && connectedUsers[i]->GetPort() == port)
				{
					sendingUser = connectedUsers[i];
					break;
				}
				else if (i == connectedUsers.size() - 1)
				{
					User* newUser = new User();
					newUser->SetAdress(sender);
					newUser->SetPort(port);
					newUser->SetName("Anon" + std::to_string(connectedUsers.size() + 1));
					connectedUsers.push_back(newUser);
					sendingUser = newUser;
				}
			}
		}
		else 
		{
			User* newUser = new User();
			newUser->SetAdress(sender);
			newUser->SetPort(port);
			newUser->SetName("Anon1");
			connectedUsers.push_back(newUser);
			sendingUser = newUser;
		}


		// Send an answer
		std::stringstream mstream;
		mstream.clear();
		if (!bufferStr.empty() && sendingUser != nullptr) {
			mstream << sendingUser->GetName() << " said: " << buffer << std::endl;
			std::string message = mstream.str();

			std::cout << message;
			for (int i = 0; i < connectedUsers.size(); i++)
			{
				if (!message.empty() && (connectedUsers[i]->GetAdress() != sender && connectedUsers[i]->GetPort() != port))
					socket.send(message.c_str(), message.size() + 1, connectedUsers[i]->GetAdress(), connectedUsers[i]->GetPort());
			}
		}
	}

	for (auto it : connectedUsers) {
		delete it;
		it = nullptr;
	}

	connectedUsers.clear();

	return 0;
}