#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <SFML\Network.hpp>

class HistoryLog
{
public:
	HistoryLog();
	~HistoryLog();

	void				CreateFile	(const std::string filename);

	const std::string	GetTimeStamp() const;
	
};

