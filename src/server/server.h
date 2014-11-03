#ifndef SERVER_H
#define SERVER_H

#include "client.h"

#include <sys/socket.h>
#include <netinet/in.h>

#include <vector>
#include <atomic>
#include <mutex>

namespace Wimf {

class Server
{
private:
	sockaddr_in serv_addr;
	std::vector<Client> clients;

	std::mutex start_stop;
	std::atomic_bool runserv;

	int sock_fd;

public:
	Server (int port);
	virtual ~Server ();

	void start ();
	void stop ();
};

}

#endif
