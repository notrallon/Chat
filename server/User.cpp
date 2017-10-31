#include "User.h"

User::User()
{
}

User::~User()
{
}

const std::string & User::GetName() const 
{
	return m_Username;
}

void User::SetName(std::string name)
{
	m_Username = name;
}

const std::string & User::GetAdress() const
{
	return m_Adress;
}

void User::SetAdress(std::string ip)
{
	m_Adress = ip;
}
