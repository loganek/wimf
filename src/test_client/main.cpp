/*
 * main.cpp
 *
 *  Created on: Aug 24, 2015
 *      Author: loganek
 */

#include "protocol/protocol.h"
#include "protocol/wimf.pb.h"

#include <netdb.h>

#include <thread>
#include <iostream>

void error(const char *msg)
{
	std::cerr << msg << std::endl;
	exit(1);
}

int main (int argc, char **argv)
{
	if (argc != 4)
	{
		error(("Usage: " + std::string(argv [0]) + " <nickname> <server ip> <port>").c_str());
		return 1;
	}

	std::string nickname = argv [1];
	sockaddr_in serv_addr;

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

	Wimf::Protocol protocol (sockfd, [](const WimfInfo& info){
		if (info.has_message())
			std::cout << "Have message " << info.message().text() << std::endl;
		else if (info.has_login())
			std::cout << "Login confirmed: " << info.login().nickname() << " " << info.login().id() << std::endl;
		else
			std::cout << "unknown frame" << std::endl;
	});

	std::thread th([protocol] {
		while (protocol.read_frame());
	});

	WimfInfo info;
	info.mutable_login()->set_nickname(nickname);
	protocol.send_frame(info);
	int dest_id;
	do {
		std::cin >> dest_id;
		WimfInfo frame;
		auto msg = frame.mutable_message();
		msg->set_to(dest_id);
		msg->set_text("hello ;)");
		protocol.send_frame(frame);
	} while (dest_id != -1);

	shutdown (sockfd, SHUT_RDWR);
	th.join();

	return 0;
}

