#include "HistoryLog.h"



HistoryLog::HistoryLog()
{
}


HistoryLog::~HistoryLog()
{
}

void HistoryLog::CreateFile()
{
	std::ofstream outfile("test.txt");

	outfile << "my text here!" << std::endl;

	outfile.close();
}