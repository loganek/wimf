#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"

#include <string>
#include <memory>

namespace Wimf {

class Client
{
 private:
  int fd;

  std::shared_ptr<User> user;

  void process_queries ();
  void close ();

 public:
  Client (int client_fd);
  virtual ~Client () {}

  void start ();
  void send_message (const std::string& message) const;
};

}

#endif
