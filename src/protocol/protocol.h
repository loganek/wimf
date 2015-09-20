/*
 * protocol.h
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "protocol/wimf.pb.h"

#include <functional>

namespace Wimf {

class Protocol
{
	typedef std::function<void(const eu::onionsoftware::wimf::WimfInfo&)> frame_callback;

	// frame header (frame size: 4 bytes)
	static constexpr int header_size = 4;

	int sock_fd;
	frame_callback callback;

	bool read_requested_size (int requested_size, char* buffer) const;
	bool write_requested_size (int requested_size, char* buffer) const;

public:
	Protocol (int sock_fd, frame_callback callback);
	virtual ~Protocol () {}

	bool read_frame () const;
	bool send_frame (const eu::onionsoftware::wimf::WimfInfo &frame) const;
};

}

#endif /* PROTOCOL_H_ */
