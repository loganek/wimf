#include "server.h"
#include "protocol/logger.h"

int main (int argc, char **argv)
{
	if (argc < 2)
	{
		Wimf::Logger::log ("and port?");
		return 0;
	}

	auto server = std::make_shared<Wimf::Server> (atoi (argv[1]));

	server->start ();

	return 0;
}
