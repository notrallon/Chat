
#include "ChatWindow.h"

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
	ChatWindow chatwindow;

	std::string str = "asdf";
	
	while (true)
	{
		chatwindow.render();
		chatwindow.add_string(str);

		str += str;

		
	}

	return 0;
}