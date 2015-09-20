#include "server.h"
#include "protocol/logger.h"

#include <google/protobuf/stubs/common.h>

#include <signal.h>
#include <thread>

std::shared_ptr<Wimf::Server> server;

void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		if (server) server->stop();
	}
}

int main (int argc, char **argv)
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if (argc < 2)
	{
		Wimf::Logger::log ("Usage: " + std::string (argv[0]) + " <port>");
		return 0;
	}

	if (signal(SIGINT, sig_handler) == SIG_ERR)
		Wimf::Logger::log ("cannot catch SIGINT");

	server = std::make_shared<Wimf::Server> (atoi (argv[1]));

	std::thread th ([]{ server->start (); });

	th.join();

	return 0;
}
