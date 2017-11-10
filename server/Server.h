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
	using CommandMap = std::map<std::string, std::pair<std::function<void(User*, Server*, std::string)>, std::string>>;
	using CommandIterator = CommandMap::iterator;
	using CommandValType = CommandMap::value_type;

	friend struct ComFuncs;
public:
	Server();
	~Server();

	void Run();

private:
	static HistoryLog	sm_historyLog;
	sf::UdpSocket		m_socket;
	CommandMap			m_Commands;
	UserMap				m_Users;

	void				CreateUser(const sf::IpAddress sender, const unsigned short port, User*& sendingUser, std::string username);
	void				SendToAll(std::string message);
	void				CheckUsersConnected(sf::Time time);

	//					Server_Commands (can be found in Server_Commands.cpp)
	void				ChangeUsername(User* sender, std::string buffer);
	void				DisconnectUser(std::string username);
	void				WhisperUser(User* sender, std::string buffer);
	void				PrintCommands(User* sender);

};



