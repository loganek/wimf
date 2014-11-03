/*
 * messageframe.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#include "messageframe.h"

#include <stdexcept>
#include <cassert>

using namespace Wimf::DataFrames;

template<typename T>
T convert_to(const Wimf::data_array& data, Wimf::data_array::size_type start)
{
	assert (data.size () >= start + sizeof (T));

	return *((T*)data.data ());
}

MessageFrame::MessageFrame (user_id from, user_id to, const std::string& message)
: from (from), to (to), message (message)
{}

std::shared_ptr<IDataFrame> MessageFrame::parse_frame (const data_array& data, data_array::size_type pointer)
{
	constexpr data_array::size_type min_size = sizeof (user_id) * 2 + 1;

	if (pointer < min_size)
		return 0;

	user_id from = convert_to<user_id> (data, pointer);
	user_id to = convert_to<user_id> (data, pointer + sizeof (user_id));
	data_type msg_len = data [min_size - 1];

	if (msg_len + min_size != pointer)
		throw std::runtime_error ("message length not equals...");

	std::string message;
	message.assign ((const char*)(data.data () + min_size), msg_len);

	auto frame = new MessageFrame (from, to, message);

	return std::shared_ptr<IDataFrame> (frame);
}


