#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <string>

sf::UdpSocket socket;

void ReceiveMessage() 
{
	while (true) 
	{
		// Receive an answer (most likely from 192.168.1.50, but could be anyone else)
		char buffer[1024];
		std::size_t received = 0;
		sf::IpAddress sender;
		unsigned short port;
		socket.receive(buffer, sizeof(buffer), received, sender, port);

		std::cout << buffer << std::endl;
	}
}

int main() 
{
	// ----- The client -----
	// Create a socket and bind it to the port 55001
	socket.bind(socket.AnyPort);

	// Send a message to 192.168.1.50 on port 55002
	std::string serverIP = sf::IpAddress::getLocalAddress().toString();
	
	std::cout << "What's the IP you would like to connect to?";
	std::cin >> serverIP;

	std::string messageAdder;

	std::thread receive(ReceiveMessage);
	while (true) 
	{
		std::getline(std::cin, messageAdder);
		socket.send(messageAdder.c_str(), messageAdder.size() + 1, serverIP, 55002);
	}

	return 0;
}