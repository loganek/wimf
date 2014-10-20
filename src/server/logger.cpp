#include "logger.h"

#include <iostream>

using namespace Wimf;

void Logger::log (const std::string& message)
{
  std::cout << message << std::endl;
}
