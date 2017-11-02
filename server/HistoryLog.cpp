#include "HistoryLog.h"



HistoryLog::HistoryLog()
{
}


HistoryLog::~HistoryLog()
{
}

void HistoryLog::CreateFile(const std::string filename)
{
	std::ofstream outfile;

	//Open .txt file with filename (or create it if it doesnt exist)
	//std::ios::app opens the file with appending mode. Meaning it adds text to the end of the file.
	outfile.open(filename + ".txt", std::ios::app);
	//Writes the text to be written inside said file
	outfile << GetTimeStamp() << std::endl;
	//Closes the file. Stopping further writing.
	outfile.close();
}

const std::string HistoryLog::GetTimeStamp() const
{
	//Gets time in seconds since 1970
	time_t nowTime = time(NULL);

	//size 26 is exact amount of needed size
	char str[26];
	std::string timeResult;

	//str becomes something similiar to 'Wed Nov  1 16:10:05 2017'
	ctime_s(str, sizeof str, &nowTime);
	//store the char as a string instead
	timeResult = str;
	//return our string. our timestamp
	return "["+timeResult+"]";
}


