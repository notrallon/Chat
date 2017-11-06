#pragma once

#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <string>

class Client {
public:
					Client();
					~Client();

	void			Run();

private:
	static void		ReceiveMessage(sf::UdpSocket& socket);

	sf::UdpSocket	m_Socket;
	std::string		m_Name;
};