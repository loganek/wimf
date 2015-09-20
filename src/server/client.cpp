#include "client.h"
#include "server.h"
#include "protocol/logger.h"
#include "protocol/wimf.pb.h"

#include <unistd.h>

using namespace eu::onionsoftware::wimf;
using namespace Wimf;

Client::Client (int client_fd, std::shared_ptr<Server> parent_server)
: fd (client_fd),
  parent (parent_server),
  protocol (client_fd, std::bind (&Client::on_new_frame, this, std::placeholders::_1))
{
}

void Client::start ()
{ 
	while (protocol.read_frame ());
	close ();
}

void Client::close ()
{
	Logger::log ("Client: disconnecting");
	parent->remove_client (fd);
	::close (fd);
}

void Client::on_new_frame (const WimfInfo& frame)
{
	Wimf::Logger::log ("Client: have a new frame");

	if (frame.has_login ())
		login_frame (frame.login ());

	if (!logged_in ())
	{
		Wimf::Logger::log ("Client: cannot process frame. Client is not logged in");
		return;
	}

	if (frame.has_message ())
		message_frame (frame.message ());
	else if (frame.has_location ())
		location_frame (frame.location ());
}

void Client::message_frame (const Message& frame)
{
	auto to_client = parent->get_client (frame.to ());

	if (!to_client)
	{
		Wimf::Logger::log ("Client: cannot pass message. 'To' client does not exist");
		return;
	}

	WimfInfo wimf_frame;
	auto msg = wimf_frame.mutable_message ();
	msg->CopyFrom (frame);
	msg->set_from (user->get_id ());

	to_client->send_frame(wimf_frame);
}

void Client::login_frame (const Login& frame)
{
	if (!logged_in ())
	{
		user = std::make_shared<User> (fd, frame.nickname());

		WimfInfo info;
		Login *confirmation = info.mutable_login();
		confirmation->set_id(fd);
		confirmation->set_nickname(frame.nickname());

		protocol.send_frame(info);
	}
}

void Client::location_frame (const Location& frame)
{
	user->set_coords (frame.latitude (), frame.longitude ());

	// todo send info to previous neighbors, that client disappeared from the region
	parent->broadcast_new_location (user);
}

void Client::send_frame (const WimfInfo &frame) const
{
	protocol.send_frame (frame);
}
