#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"
#include "protocol.h"
#include "data_frames/dataframes.h"

#include <string>
#include <memory>

namespace Wimf {

class Server;

class Client
{
private:
	int fd;

	std::shared_ptr<User> user;
	std::shared_ptr<Server> parent;
	Protocol protocol;

	void message_frame (std::shared_ptr<DataFrames::MessageFrame> frame);
	void on_new_frame (std::shared_ptr<DataFrames::IDataFrame> frame);

	void process_queries ();
	void close ();

public:
	Client (int client_fd, std::shared_ptr<Server> parent_server);
	virtual ~Client () {}

	void start ();
	void send_message (const std::string& message) const;
};

}

#endif
