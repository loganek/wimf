/*
 * messageframe.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#include "messageframe.h"

#include <cstring>
#include <stdexcept>
#include <cassert>

using namespace Wimf::DataFrames;

template<typename T>
T convert_to(const Wimf::DataBuffer& buffer, Wimf::size_type start)
{
	assert (buffer.get_size () >= start + sizeof (T));

	return *((T*)buffer.get_data());
}

MessageFrame::MessageFrame (user_id from, user_id to, const std::string& message)
: from (from), to (to), message (message)
{}

std::shared_ptr<IDataFrame> MessageFrame::parse_frame (const Wimf::DataBuffer& buffer)
{
	constexpr size_type min_size = sizeof (user_id) * 2 + 1;

	if (buffer.get_pointer () < min_size)
		return 0;

	user_id from = convert_to<user_id> (buffer, 0);
	user_id to = convert_to<user_id> (buffer, sizeof (user_id));
	data_type msg_len = buffer [min_size - 1];

	if (msg_len + min_size != buffer.get_pointer ())
		throw std::runtime_error ("message length not equals...");

	std::string message;
	message.assign ((const char*)(buffer.get_data () + min_size), msg_len);

	auto frame = new MessageFrame (from, to, message);

	return std::shared_ptr<IDataFrame> (frame);
}


Wimf::DataBuffer MessageFrame::serialize ()
{
	Wimf::DataBuffer ret_data;
	ret_data.append (static_cast<data_type> (FrameType::MESSAGE));

	for (size_t i = 0; i < sizeof (from); i++)
		ret_data.append (((data_type*)&from) [i]);
	for (size_t i = 0; i < sizeof (to); i++)
		ret_data.append (((data_type*)&to) [i]);

	ret_data.append (message.length ());

	for (auto c : message)
		ret_data.append (c);

	return ret_data;
}
