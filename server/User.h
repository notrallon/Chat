#pragma once

#include <string>
#include <SFML/Network.hpp>
#include "HistoryLog.h"

using uShort = unsigned short;

class User {
public:
							User();
							~User();

	const std::string&		GetName() const;
	void					SetName(const std::string& name);

	const sf::IpAddress&	GetAdress() const;
	void					SetAdress(const sf::IpAddress& ip);

	const uShort&			GetPort() const;
	void					SetPort(const uShort& port);

	const std::string&		UserInfo();

	const sf::Time&			GetTime() const;
	void					SetTime(sf::Time time);

private:
	std::string				m_Username;
	sf::IpAddress			m_Adress;
	uShort					m_Port;

	sf::Time				m_Time;

	std::string				m_UserInfo;
};