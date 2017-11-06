#pragma once
#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <map>
#include <functional>

#include "HistoryLog.h"
#include "User.h"

struct ComFuncs 
{
	void SetName(std::string buffer, User* user);
};

class Server
{
public:
	Server();
	~Server();

	void Run();

private:
	using CommandMap = std::map<std::string, std::function<void(std::string, User*)>>;

	static HistoryLog	sm_historyLog;
	sf::UdpSocket		m_socket;
	std::vector<User*>	m_connectedUsers;
	CommandMap			m_Commands;

	void CreateUser(const sf::IpAddress sender, const unsigned short port, User*& sendingUser);
};

