#include <iostream>
#include <SFML/Network.hpp>

int main() 
{
	// ----- The client -----
	// Create a socket and bind it to the port 55001
	sf::UdpSocket socket;
	socket.bind(socket.AnyPort);

	int localPort = socket.getLocalPort();

	// Send a message to 192.168.1.50 on port 55002
	std::string serverIP = sf::IpAddress::getLocalAddress().toString();
	
	std::cout << "What's the IP you would like to connect to?";
	std::cin >> serverIP;

	
	
	char messageAdder[256];

	while (true) {
		std::cin.clear();
		std::cin.ignore(10000, '\n');

		std::cout << "Your message?";
		std::cin.getline(messageAdder, 256);

		socket.send(messageAdder, sizeof(messageAdder) + 1, serverIP, 55002);

		// Receive an answer (most likely from 192.168.1.50, but could be anyone else)
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);
		std::cout << buffer << std::endl;
	}

	return 0;
}