#include "server.h"
#include "logger.h"

int main (int argc, char **argv)
{
	if (argc < 2)
	{
		Wimf::Logger::log ("and port?");
		return 0;
	}

	Wimf::Server server (atoi (argv[1]));

	server.start ();

	return 0;
}
