#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace Wimf {

class Logger
{
 public:
  virtual ~Logger () {}
  
  static void log (const std::string& message);
};

}

#endif
