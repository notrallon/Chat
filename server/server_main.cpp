#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>

int main() 
{
	// ----- The server -----
	// Create a socket and bind it to the port 55002
	sf::UdpSocket socket;
	socket.bind(55002);

	std::vector<sf::IpAddress> connectedIPs;

	while (true) {
		// Receive a message from anyone
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);

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

		std::cout << sender.toString() << " said: " << buffer << std::endl;

		// Send an answer
		std::stringstream mstream;
		mstream << sender.toString() << " said: " << buffer << std::endl;
		std::string message = mstream.str();

		for (int i = 0; i < connectedIPs.size(); i++) 
		{
			socket.send(message.c_str(), message.size() + 1, connectedIPs[i], port);
		}
	}

	return 0;
}