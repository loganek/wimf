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


std::tuple<Wimf::data_array, Wimf::data_array::size_type> MessageFrame::serialize ()
{
	std::tuple<data_array, data_array::size_type> ret_data;
	data_array::size_type i = 0;

	std::get<0> (ret_data) [i++] = static_cast<data_type> (FrameType::MESSAGE);

	memcpy (std::get<0> (ret_data).data () + i, (data_type*)&from, sizeof (from));
	i += sizeof (from);
	memcpy (std::get<0> (ret_data).data () + i, (data_type*)&to, sizeof (to));
	i += sizeof (to);
	std::get<0> (ret_data) [i++] = message.length ();

	for (auto c : message)
		std::get<0> (ret_data) [i++] = c;

	std::get<1> (ret_data) = 1 + sizeof (from) + sizeof (to) + 1 + message.length ();

	return ret_data;
}
