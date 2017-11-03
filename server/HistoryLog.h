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
	void				AddTextLog		(const std::string filename, const std::string& message);

private:
	
	//Format = 'Wed Nov  1 16:10:05 2017'
	const std::string	GetTimeStamp	() const;
	
};

