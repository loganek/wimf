#ifndef SERVER_H
#define SERVER_H

#include "client.h"

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
	std::map<int, std::shared_ptr<Client>> clients;

	std::mutex start_stop;
	std::atomic_bool runserv;

	int sock_fd;

public:
	Server (int port);
	virtual ~Server ();

	void start ();
	void stop ();

	std::shared_ptr<Client> get_client (user_id id);
	std::vector<std::shared_ptr<Client>> get_clients_from_location (double latitude, double longitude);

	void remove_client (int sock_fd);
};

}

#endif
