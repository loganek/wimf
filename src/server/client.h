#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"
#include "protocol/protocol.h"
#include "protocol/wimf.pb.h"

#include <string>
#include <memory>

namespace Wimf {

class Server;

class Client : public std::enable_shared_from_this<Client>
{
private:
	int fd;

	std::shared_ptr<User> user;
	std::shared_ptr<Server> parent;
	Protocol protocol;

	void message_frame (const eu::onionsoftware::wimf::Message& frame);
	void login_frame (const eu::onionsoftware::wimf::Login& frame);
	void location_frame (const eu::onionsoftware::wimf::Location& frame);
	void user_info_frame (const eu::onionsoftware::wimf::UserInfo& frame);

	void on_new_frame (const eu::onionsoftware::wimf::WimfInfo& frame);

	void close ();

	bool logged_in() const { return !!user; }

public:
	Client (int client_fd, std::shared_ptr<Server> parent_server);
	virtual ~Client () {}

	void start ();

	void send_frame (const eu::onionsoftware::wimf::WimfInfo &frame) const;

	std::shared_ptr<User> get_user () const { return user; }
};

}

#endif
