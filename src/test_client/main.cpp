/*
 * main.cpp
 *
 *  Created on: Aug 24, 2015
 *      Author: loganek
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <iostream>

void error(const char *msg)
{
	std::cerr << msg << std::endl;
	exit(1);
}

int send_hello (int sock_fd, std::int64_t id)
{
	char data [sizeof (id) + 2];
	size_t i = 1;
	data [0] = 1;

	for (; i < sizeof (id)+1; i++)
		data [i] = (((char*)&id) [i-1]);

	data [i] = 0xFF ^ 220;
	return write (sock_fd, data, sizeof (id) + 2);
}

int main (int argc, char **argv)
{
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv [0] << " <user id> <server ip> <port>" << std::endl;
		return 1;
	}

	sockaddr_in serv_addr;
	char buffer[256];

	int sockfd = socket (AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
	}

	hostent *server = gethostbyname(argv[2]);
	if (server == nullptr)
	{
		error ("ERROR, no such host\n");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(atoi(argv[3]));

	if (connect (sockfd, (sockaddr *) &serv_addr,sizeof (serv_addr)) < 0)
	{
		error ("ERROR connecting");
	}

	if (send_hello (sockfd, atoi (argv [1])) < 0)
	{
		error ("ERROR writing to socket HELLO message");
	}

	bzero (buffer, 256);
	if (read (sockfd, buffer, 255) < 0)
	{
		error ("ERROR reading from socket");
	}
	std::cout << buffer << std::endl;
	close (sockfd);

	return 0;
}

