/*
 * main.cpp
 *
 *  Created on: Aug 24, 2015
 *      Author: loganek
 */

#include "protocol/protocol.h"
#include "protocol/wimf.pb.h"
#include "../server/user.h"

#include <netdb.h>

#include <thread>
#include <iostream>

using namespace eu::onionsoftware::wimf;

void error(const char *msg)
{
	std::cerr << msg << std::endl;
	exit(1);
}

std::map<std::int32_t, std::shared_ptr<Wimf::User>> people;

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

void list_command()
{
	for (auto p : people)
	{
		std::cout << p.second->get_id() << " " << p.second->get_nickname() << " " << p.second->get_latitude() << " " << p.second->get_longitude() << std::endl;
	}
}

void request_username(std::int32_t id, const Wimf::Protocol& protocol)
{
	WimfInfo frame;
	frame.mutable_user_info()->set_id(id);
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

	Wimf::Protocol protocol (sockfd, [&protocol](const WimfInfo& info){
		if (info.has_message())
			std::cout << "Have message " << info.message().text() << std::endl;
		else if (info.has_login())
			std::cout << "Login confirmed: " << info.login().nickname() << " " << info.login().id() << std::endl;
		else if (info.has_location ())
		{
			std::cout << "New user location: " << info.location().user_id() << " " << info.location().latitude() << " " << info.location().longitude() << std::endl;
			if (people.find (info.location().user_id()) == people.end())
			{
				people.emplace(info.location().user_id(), std::make_shared<Wimf::User>(info.location().user_id(), "unknown"));
				request_username(info.location().user_id(), protocol);
			}
			people[info.location().user_id()]->set_coords(info.location().latitude(), info.location().longitude());
		}
		else if (info.has_user_info())
		{
			auto id = info.user_info().id();
			double prev_lat, prev_lon; bool update_coords = false;
			if (people.find(id) != people.end())
			{
				prev_lat = people[id]->get_latitude();
				prev_lon = people[id]->get_longitude();
				update_coords = true;
			}
			people[id] = std::make_shared<Wimf::User>(id, info.user_info().login().nickname());
			if (update_coords) people[id]->set_coords(prev_lat, prev_lon);
		}
		else if (info.has_availability())
		{
			if (info.availability().status() != Availability_Status_IN_RANGE)
			{
				people.erase(info.availability().user_id());
			}
		}
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
		else if (command == "list")
			list_command();
	}

	shutdown (sockfd, SHUT_RDWR);
	th.join();

	return 0;
}

