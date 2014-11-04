/*
 * protocol.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#include "protocol.h"
#include "logger.h"
#include "data_frames/dataframes.h"

#include <cstdint>

using namespace Wimf;

Protocol::Protocol (frame_callback callback)
: callback (callback)
{
}

void Protocol::append_byte(const data_type& byte)
{
	data [data_pointer++] = byte;

	if (is_frame_over ())
	{
		parse_frame ();
		data_pointer = 0;
	}

	if (data_pointer == data.size () / 2)
	{
		Logger::log ("protocol data overflow");
		data_pointer = 0;
	}
}

bool Protocol::is_frame_over () const
{
	if (data_pointer < 2 || data [data_pointer - 1] != end_frame)
		return false;

	bool c = true;

	for (auto it = data.rbegin () - 1; it != data.rend (); ++it)
	{
		if (*it != guard)
			break;

		c = !c;
	}

	return c;
}

void Protocol::parse_frame ()
{
	FrameType type = preprocess_frame ();
	std::shared_ptr<DataFrames::IDataFrame> frame;

	switch (type)
	{
	case FrameType::MESSAGE:
		frame = DataFrames::MessageFrame::parse_frame (data, data_pointer);
		break;
	}

	if (frame)
	{
		Logger::log ("frame parsed");
		callback (frame);
	}
}

// todo has to be optimized...
FrameType Protocol::preprocess_frame ()
{
	data_array new_data;
	bool prev_guard = false;
	data_array::size_type ni = 0;

	auto type = static_cast<FrameType> (data [0]);

	for (data_array::size_type i = 1; i < data_pointer - 1; ++i)
	{
		if (prev_guard)
		{
			if (data [i] == guard || data [i] == end_frame)
			{
				new_data [ni++] = guard;
				prev_guard = false;
				continue;
			}
			else
				throw std::runtime_error ("syntax error");
		}
		if (data [i] == guard)
			prev_guard = true;
		else
			new_data [ni++] = data [i];
	}

	if (prev_guard)
		throw std::runtime_error ("syntax error");

	data = new_data;
	data_pointer = ni;

	return type;
}

data_array::size_type Protocol::postserialize (data_array& data)
{
	data_array new_data;
	data_array::size_type ni = 0;

	for (data_array::size_type i = 0; i < data.size (); i++)
	{
		if (data [i] == guard || data [i] == end_frame)
			new_data [ni++] = guard;
		new_data [ni++] = data [i];
	}

	new_data [ni++] = end_frame;
	data = new_data;
	return ni;
}
