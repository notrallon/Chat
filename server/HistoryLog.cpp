#include "HistoryLog.h"

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

HistoryLog::HistoryLog()
{
}
HistoryLog::~HistoryLog()
{
}

void HistoryLog::AddTextLog(const std::string filename, const std::string message)
{
	std::ofstream outfile;

	//Open .txt file with filename (or create it if it doesnt exist)
	//std::ios::app opens the file with appending mode. Meaning it adds text to the end of the file.
	outfile.open(filename + ".txt", std::ios::app);
	//Writes the text to be written inside said file
	outfile << "[" << GetTimeStamp() << "]" << message << std::endl;
	//Closes the file. Stopping further writing.
	outfile.close();
}

const std::string HistoryLog::GetTimeStamp() const
{
	std::string timeResult;
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%F %X", timeinfo);
	puts(buffer); //<- dunno what this does

	timeResult = buffer;
	//return our string. our timestamp
	return timeResult;
}


