#ifndef SERVER_H
#define SERVER_H

#include "client.h"

#include <boost/optional/optional.hpp>

#include <sys/socket.h>
#include <netinet/in.h>

#include <map>
#include <atomic>
#include <mutex>

namespace Wimf {

class Server : public std::enable_shared_from_this<Server>
{
private:
	sockaddr_in serv_addr;
	std::map<int, Client> clients;

	std::mutex start_stop;
	std::atomic_bool runserv;

	int sock_fd;

public:
	Server (int port);
	virtual ~Server ();

	void start ();
	void stop ();

	boost::optional<Client> get_client (user_id id);
};

}

#endif
