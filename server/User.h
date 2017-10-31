#pragma once

#include <string>

class User {
public:
						User();
						~User();

	const std::string&	GetName() const;
	void				SetName(std::string name);

	const std::string&	GetAdress() const;
	void				SetAdress(std::string ip);

private:
	std::string			m_Username;
	std::string			m_Adress;
};