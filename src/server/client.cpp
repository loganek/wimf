#include "client.h"
#include "server.h"

#include <sys/socket.h>

#include <unistd.h>
#include <cstdio>

using namespace Wimf;

Client::Client (int client_fd, std::shared_ptr<Server> parent_server)
: fd (client_fd),
  parent (parent_server),
  protocol (std::bind(&Client::on_new_frame, this, std::placeholders::_1))
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

		for (int i = 0; i < n; ++i)
			protocol.append_byte (buff [i]);
	}

	close ();
}

void Client::send_message (std::shared_ptr<DataFrames::IDataFrame> frame) const
{
	DataBuffer buff = frame->serialize();

	send (fd, buff.get_data(), buff.get_pointer(), 0);
}

void Client::process_queries ()
{
}

void Client::close ()
{
	::close (fd);
}

void Client::on_new_frame (std::shared_ptr<DataFrames::IDataFrame> frame)
{
	switch (frame->get_frame_type ())
	{
	case FrameType::MESSAGE:
		message_frame (std::static_pointer_cast<DataFrames::MessageFrame> (frame));
		break;
	case FrameType::HELLO:
		hello_frame (std::static_pointer_cast<DataFrames::HelloFrame> (frame));
	}
}

void Client::message_frame (std::shared_ptr<DataFrames::MessageFrame> frame)
{
	auto client = parent->get_client (frame->get_to ());

	if (!client)
	{
		return;
	}

	client.get().send_message (frame);
}

void Client::hello_frame (std::shared_ptr<DataFrames::HelloFrame> frame)
{
	user = std::make_shared<User> (frame->get_id ());
}
