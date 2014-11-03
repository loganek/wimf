#include "client.h"

#include <sys/socket.h>

#include <unistd.h>
#include <cstdio>

using namespace Wimf;

Client::Client (int client_fd)
: fd (client_fd)
{
}

void Client::start ()
{ 
	while (true)
	{
		char buff[256] = {0};
		int n = recv (fd, buff, 255, 0);

		if (n <= 0)
			break;

	}

	close ();
}

void Client::send_message (const std::string& message) const
{
	send (fd, message.c_str(), message.length(), 0);
}

void Client::process_queries ()
{
}

void Client::close ()
{
	::close (fd);
}
