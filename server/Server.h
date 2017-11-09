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
	//using CommandMap = std::map<std::string, std::function<void(std::string, User*, Server*)>>;
	using CommandMap = std::map<std::string, std::pair<std::function<void(User*, Server*)>, std::string>>;
	using ValueCommandMap = std::map<std::string, std::pair<std::function<void(std::string, User*, Server*)>, std::string>>;

	static HistoryLog	sm_historyLog;
	sf::UdpSocket		m_socket;
	//std::vector<User*>	m_connectedUsers;
	CommandMap			m_Commands;
	ValueCommandMap		m_ValueCommands;
	UserMap				m_Users;

	void				CreateUser(const sf::IpAddress sender, const unsigned short port, User*& sendingUser);
	void				CreateUser(const sf::IpAddress sender, const unsigned short port, User*& sendingUser, std::string username);
	void				SendToAll(std::string message);
	void				CheckUsersConnected(sf::Time time);

	//					Server_Commands (can be found in Server_Commands.cpp)
	void				ChangeUsername(User* sender, std::string buffer);
	void				DisconnectUser(User* user);
	void				WhisperUser(User* sender, std::string buffer);
	void				PrintCommands(User* sender);

};



