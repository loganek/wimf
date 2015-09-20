/*
 * protocol.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#include "protocol.h"
#include "logger.h"

#include <sys/socket.h>

using namespace Wimf;

Protocol::Protocol (int sock_fd, frame_callback callback)
: sock_fd (sock_fd),
  callback (callback)
{
}

bool Protocol::read_requested_size (int requested_size, char* buffer) const
{
	int size = 0;
	int cnt = 0;

	while (size < requested_size)
	{
		if ((cnt = recv (sock_fd, buffer + size, requested_size - size, 0)) < 0)
		{
			return false;
		}
		size += cnt;
	}

	return true;
}

bool Protocol::write_requested_size (int requested_size, char* buffer) const
{
	int size = 0;
	int cnt = 0;

	while (size < requested_size)
	{
		if ((cnt = send (sock_fd, buffer + size, requested_size - size, 0)) < 0)
		{
			return false;
		}
		size += cnt;
	}

	return true;
}

bool Protocol::read_frame () const
{
	constexpr int static_buff_size = 256;
	constexpr int max_frame_size = 4096;
	char static_buff[static_buff_size] = {0};
	char *buff = static_buff;

	if (!read_requested_size (header_size, buff))
	{
		Logger::log ("Protocol: cannot read frame header");
		return false;
	}

	int frame_size = 0;
	for (int i = 0; i < header_size; i++)
	{
		frame_size |= buff[i] << (i << 3);
	}

	if (frame_size < max_frame_size)
	{
		Logger::log ("Protocol: frame_size is greater than max_frame_size");
		return false;
	}

	if (static_buff_size < frame_size)
		buff = new char[frame_size];

	if (!read_requested_size (frame_size, buff))
	{
		Logger::log ("Protocol: cannot read frame");
		return false;
	}

	WimfInfo info;
	if (!info.ParseFromArray (buff, frame_size))
	{
		Logger::log ("Protocol: cannot deserialize frame");
		return false;
	}

	Logger::log ("Protocol: frame has been received, processing...");

	callback(info);

	if (static_buff != buff)
		delete buff;

	return true;
}

bool Protocol::send_frame (const WimfInfo &frame) const
{
	int size = frame.ByteSize();
	char header[4];

	for (int i = 0; i < header_size; i++)
	{
		header[i] = size & 255;
		size >>= 8;
	}

	if (!write_requested_size (header_size, header))
	{
		Logger::log ("Protocol: cannot send header");
		return false;
	}
	if (!frame.SerializeToFileDescriptor (sock_fd))
	{
		Logger::log ("Protocol: cannot send frame");
		return false;
	}

	return true;
}
