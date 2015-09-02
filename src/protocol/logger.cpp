#include "logger.h"

#include <ctime>
#include <iostream>

using namespace Wimf;

void Logger::log (const std::string& message)
{
	time_t now = time(0);
	tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

	std::cout << "[" << buf << "] " << message << std::endl;
}
