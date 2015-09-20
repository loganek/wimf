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

using namespace eu::onionsoftware::wimf;

void error(const char *msg)
{
	std::cerr << msg << std::endl;
	exit(1);
}

void message_command(const Wimf::Protocol& protocol)
{
	int dest_id;
	std::cin >> dest_id;
	WimfInfo frame;

	auto msg = frame.mutable_message();
	msg->set_to(dest_id);
	msg->set_text("hello ;)");
	protocol.send_frame(frame);
}

void location_command(const Wimf::Protocol& protocol)
{
	WimfInfo frame;
	double lat, lon;
	std::cin >> lat >> lon;

	auto loc = frame.mutable_location();
	loc->set_latitude(lat);
	loc->set_longitude(lon);
	protocol.send_frame(frame);
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
		else if (info.has_location ())
			std::cout << "New user location: " << info.location().user_id() << " " << info.location().latitude() << " " << info.location().longitude() << std::endl;
		else
			std::cout << "unknown frame" << std::endl;
	});

	std::thread th([protocol] {
		while (protocol.read_frame());
	});

	WimfInfo info;
	info.mutable_login()->set_nickname(nickname);
	protocol.send_frame(info);
	std::string command;

	while (true)
	{
		std::cin >> command;
		if (command == "exit")
			break;
		else if (command == "message")
			message_command(protocol);
		else if (command == "location")
			location_command(protocol);
	}

	shutdown (sockfd, SHUT_RDWR);
	th.join();

	return 0;
}

