/*
 * main.cpp
 *
 *  Created on: Aug 24, 2015
 *      Author: loganek
 */

#include "protocol/protocol.h"
#include "protocol/data_frames/dataframes.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <thread>
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

int send_hello (int sock_fd, Wimf::user_id id)
{
	Wimf::DataFrames::HelloFrame frame (id);
	Wimf::DataBuffer buff = frame.serialize();
	Wimf::Protocol::postserialize(buff);
	return write (sock_fd, buff.get_data(), buff.get_pointer());
}

int main (int argc, char **argv)
{
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv [0] << " <user id> <server ip> <port>" << std::endl;
		return 1;
	}

	Wimf::user_id client_id = atoi (argv [1]);
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

	if (send_hello (sockfd, client_id) < 0)
	{
		error ("ERROR writing to socket HELLO message");
	}

	std::thread th([sockfd] {
		Wimf::Protocol protocol ([](std::shared_ptr<Wimf::DataFrames::IDataFrame> frame){
			std::cout << "Have message " << (int)frame->get_frame_type() << std::endl;
			if (frame->get_frame_type() == Wimf::FrameType::MESSAGE) {
				auto msg = std::static_pointer_cast<Wimf::DataFrames::MessageFrame>(frame);
				std::cout << "Message from: " << msg->get_from() << ": " << msg->get_message() << std::endl;
			}
		});

		char buffer [256];
		int n = 0;
		while ((n = read (sockfd, buffer, 255)) >= 0)
		{
			for (int i = 0; i < n; i++)
				protocol.append_byte(buffer [i]);
		}
	});

	std::string msg = "witaj"; Wimf::user_id dest_id;
	Wimf::DataBuffer buf;
	do {
		std::cin >> dest_id;
		//std::getline (std::cin, msg);
		Wimf::DataFrames::MessageFrame msg_frame (client_id, dest_id, msg);
		buf = msg_frame.serialize();
		Wimf::Protocol::postserialize(buf);
		write (sockfd, buf.get_data(), buf.get_pointer());
	} while (dest_id != -1);

	close (sockfd);

	return 0;
}

