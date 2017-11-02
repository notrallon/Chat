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

	//Creates a .txt file (unless a .txt file already exists with the same filename).
	//Filename always includes ".txt" at the end. (dont type filetype).
	//A time stamp is set with every message given.
	//Messages are appended (added at the end of the file).
	void				AddTextLog		(const std::string filename, const std::string message);

	//Format = 2017-11-02 16:24:02
	const std::string	GetTimeStamp	() const;
	
};

