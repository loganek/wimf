#include "client.h"

#include <sys/socket.h>

#include <unistd.h>
#include <cstdio>

using namespace Wimf;

Client::Client (int client_fd)
: fd (client_fd),
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

void Client::send_message (const std::string& message) const
{
	send (fd, message.c_str(), message.length(), 0);
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
	}
}

void Client::message_frame (std::shared_ptr<DataFrames::MessageFrame> frame)
{

}
