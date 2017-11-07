#include "Client.h"
#include <consoleapi.h>

Client::Client()
{
}

Client::~Client()
{
}

void Client::Run()
{
	// ----- The client -----
	// Create a m_Socket and bind it to the port 55001
	m_Socket.bind(m_Socket.AnyPort);

	// Send a message to 192.168.1.50 on port 55002
	std::string serverIP = sf::IpAddress::getLocalAddress().toString();
	std::cout << "What's the IP you would like to connect to?";

	std::getline(std::cin, serverIP);

	std::cout << "Hello, what's your name?\n";
	std::cin >> m_Name;

	std::string messageAdder = "";

	std::thread receive(ReceiveMessage, std::ref(m_Socket));
	while (true)
	{
		messageAdder = ""; //Extra precaution
		std::getline(std::cin, messageAdder);
		if (messageAdder != "") //wont send messages if they dont have anything in them
		{
			std::string message = std::string(m_Name + " said: ");
			message += messageAdder;
			m_Socket.send(message.c_str(), message.size() + 1, serverIP, 55002);
		}
	}
}

void Client::ReceiveMessage(sf::UdpSocket& socket)
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
