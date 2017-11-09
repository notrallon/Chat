#pragma once
#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <map>
#include <functional>

#include "HistoryLog.h"
#include "User.h"

using UserMap = std::map<std::string, User*>;

class Server
{
	friend struct ComFuncs;
public:
	Server();
	~Server();

	void Run();

private:
	using CommandMap = std::map<std::string, std::function<void(std::string, User*, Server*)>>;

	static HistoryLog	sm_historyLog;
	sf::UdpSocket		m_socket;
	//std::vector<User*>	m_connectedUsers;
	CommandMap			m_Commands;
	UserMap				m_Users;

	void				CreateUser(const sf::IpAddress sender, const unsigned short port, User*& sendingUser);
	void				CreateUser(const sf::IpAddress sender, const unsigned short port, User*& sendingUser, std::string username);
	void				DisconnectUser(User* user);
	void				WhisperUser(User* sender, std::string buffer);
	void				ChangeUsername(User* sender, std::string buffer);

	void				SendToAll(std::string message);

	void				CheckUsersConnected(sf::Time time);
};



