#ifndef SERVER_H
#define SERVER_H

#include "client.h"

#include <map>

namespace Wimf {

class Server : public std::enable_shared_from_this<Server>
{
private:
	std::map<int, std::shared_ptr<Client>> clients;

	int port;
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
