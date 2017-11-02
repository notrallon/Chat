#pragma once
#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "HistoryLog.h"
#include "User.h"

class ServerStuff
{
public:
	ServerStuff();
	~ServerStuff();

	void AllServerStuff();

private:

	static HistoryLog	sm_historyLog;
	sf::UdpSocket		m_socket;
	std::vector<User*>	m_connectedUsers;

	void CreateUser(const sf::IpAddress sender, const unsigned short port, User*& sendingUser);
};

