#include "server.h"

#include "protocol/logger.h"

#include <netinet/in.h>
#include <thread>
#include <algorithm>

using namespace Wimf;
using namespace eu::onionsoftware::wimf;

Server::Server (int port)
: port (port),
  sock_fd (-1)
{
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

	sockaddr_in serv_addr;
	memset ((char*) &serv_addr, 0, sizeof (sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons (port);

	if (bind (sock_fd, (sockaddr*) &serv_addr, sizeof (serv_addr)) < 0)
		throw std::runtime_error ("cannot bind");

	listen (sock_fd, 5);

	socklen_t sockaddr_len = sizeof (sockaddr);
	sockaddr_in cli_addr;
	memset ((char*) &cli_addr, 0, sizeof (sockaddr_in));

	int client_sock_fd;

	Logger::log ("Server: accepting clients");

	while ((client_sock_fd = accept (sock_fd, (sockaddr*) &cli_addr, &sockaddr_len)) != -1)
	{
		auto c = clients.emplace (client_sock_fd, std::make_shared<Client> (client_sock_fd, shared_from_this ()));
		if (!c.second)
		{
			Logger::log ("Server: cannot add new client");
			continue;
		}
		std::thread(&Client::start, c.first->second).detach();

		Logger::log ("Server: new client connected");
	}

	Logger::log ("disconnected");
}

void Server::stop ()
{
	if (sock_fd != -1)
	{
		Logger::log ("Server: disconnecting");
		shutdown (sock_fd, SHUT_RDWR);
		sock_fd = -1;
	}
}

std::shared_ptr<Client> Server::get_client (user_id user)
{
	auto it = std::find_if (clients.begin (), clients.end () , [user] (const std::pair<int, std::shared_ptr<Client>>& c){
		auto u = c.second->get_user ();
		return (u && u->get_id() == user);
	});

	if (it != clients.end()) return it->second;

	Logger::log ("Server: Cannot find client of type: " + std::to_string (user));
	return std::shared_ptr<Client>();
}

void Server::remove_client (int sock_fd)
{
	clients.erase (sock_fd);
	Logger::log ("Server: client removed");
}

std::vector<std::shared_ptr<Client>> Server::get_clients_from_location (double latitude, double longitude)
{
	std::vector<std::shared_ptr<Client>> ok_clients;

	for (auto client : clients)
	{
		auto user = client.second->get_user ();
		if (user && user->in_range (latitude, longitude))
		{
			ok_clients.push_back (client.second);
		}
	}

	return ok_clients;
}

void Server::broadcast_new_location (const std::shared_ptr<User>& modified_user)
{
	WimfInfo frame;
	auto location = frame.mutable_location ();
	location->set_latitude (modified_user->get_latitude ());
	location->set_longitude (modified_user->get_longitude ());
	location->set_user_id (modified_user->get_id ());

	for (auto client : clients)
	{
		auto user = client.second->get_user ();
		if (user && user->in_range (modified_user->get_latitude (), modified_user->get_longitude ()))
		{
			client.second->send_frame (frame);
		}
	}
}
