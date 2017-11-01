#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include "User.h"

int main() 
{
	std::vector<User*> users;

	// ----- The server -----
	// Create a socket and bind it to the port 55002
	sf::UdpSocket socket;
	socket.bind(55002);

	const std::string SERVER_SIDE = "[SERVER->]";
	const std::string SERVER_SIDE_END = "[<-SERVER]";

	std::vector<sf::IpAddress> connectedIPs;

	while (true) { 
		// Receive a message from anyone
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);

		users.push_back(new User());
		users[users.size()-1]->SetAdress(sender.toString());
		users[users.size()-1]->SetName("User[" + std::to_string(users.size()) + "]");


		if (connectedIPs.size() > 0) 
		{
			for (int i = 0; i < connectedIPs.size(); i++) 
			{
				if (connectedIPs[i] == sender) 
				{
					break;
				}
				else if (i == connectedIPs.size() - 1) 
				{
					connectedIPs.push_back(sender);
				}
			}
		}
		else 
		{
			connectedIPs.push_back(sender);
		}

		std::cout << SERVER_SIDE << sender.toString() << " said: " << buffer << SERVER_SIDE_END << std::endl;


		for (int i = 0; i < users.size(); i++)
		{
			if (users[i]->GetAdress() == sender.toString())
			{
				// Send an answer
				std::stringstream mstream;
				mstream << SERVER_SIDE << sender.toString() << " said: " << buffer << "... also: Hello " << users[i]->GetName() << SERVER_SIDE_END << std::endl;
				std::string message = mstream.str();

				for (int i = 0; i < connectedIPs.size(); i++)
				{
					socket.send(message.c_str(), message.size() + 1, connectedIPs[i], port);
				}
			}
		}


	}

	return 0;
}