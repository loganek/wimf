#include "server.h"

int main (int argc, char **argv)
{
	Wimf::Server server (12345);

	server.start ();

	return 0;
}
