#include "server.h"
#include "protocol/logger.h"

#include <cstring>
#include <thread>
#include <stdexcept>
#include <algorithm>

using namespace Wimf;

Server::Server (int port)
: sock_fd (-1)
{
	memset ((char*) &serv_addr, 0, sizeof (sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons (port);
}

Server::~Server ()
{
	stop ();
}

void Server::start ()
{  
	sock_fd = socket (AF_INET, SOCK_STREAM, 0);

	if (sock_fd < 0)
		throw std::runtime_error ("cannot open socket");

	if (bind (sock_fd, (sockaddr*) &serv_addr, sizeof (serv_addr)) < 0)
		throw std::runtime_error ("cannot bind");

	listen (sock_fd, 5);

	socklen_t sockaddr_len = sizeof (sockaddr);
	sockaddr_in cli_addr;
	memset ((char*) &cli_addr, 0, sizeof (sockaddr_in));

	runserv = true;

	while (runserv)
	{
		std::lock_guard<std::mutex> lock (start_stop);
		int client_sock_fd = accept (sock_fd, (sockaddr*) &cli_addr, &sockaddr_len);

		if (client_sock_fd < 0)
		{
			Logger::log ("cannot accept socket");
			continue;
		}

		clients.emplace (client_sock_fd, std::make_shared<Client> (client_sock_fd, shared_from_this ()));
		std::thread(&Client::start, clients.find (client_sock_fd)->second).detach();

		Logger::log ("new client");
	}
}

void Server::stop ()
{
	runserv = false;
	std::lock_guard<std::mutex> lock (start_stop);
}

std::shared_ptr<Client> Server::get_client (user_id user)
{
	auto it = std::find_if (clients.begin (), clients.end () , [user] (const std::pair<int, std::shared_ptr<Client>>& c){
		auto u = c.second->get_user ();
		return (u && u->get_id() == user);
	});

	if (it != clients.end()) return it->second;

	Logger::log ("Cannot find client of type: " + std::to_string (user));
	return std::shared_ptr<Client>();
}

void Server::remove_client (int sock_fd)
{
	clients.erase (sock_fd);
	Wimf::Logger::log ("client removed");
}
