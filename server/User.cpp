#include "User.h"

User::User()
{
}

User::~User()
{
}

const std::string& User::GetName() const 
{
	return m_Username;
}

void User::SetName(const std::string& name)
{
	m_Username = name;
}

const sf::IpAddress& User::GetAdress() const
{
	return m_Adress;
}

void User::SetAdress(const sf::IpAddress& ip)
{
	m_Adress = ip;
}

const uShort& User::GetPort() const
{
	return m_Port;
}

void User::SetPort(const uShort& port)
{
	m_Port = port;
}

const std::string& User::UserInfo()
{
	m_UserInfo = std::string("[" + m_Adress.toString() + ":" + std::to_string(m_Port) + "]" + "[" + "Name:" + m_Username + "]");
	return m_UserInfo;
}

