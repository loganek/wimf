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
	buffer.append (byte);

	if (is_frame_over ())
	{
		parse_frame ();
		buffer.reset ();
	}

	if (buffer.get_pointer () == buffer.get_size () / 2)
	{
		Logger::log ("protocol data overflow");
		buffer.reset ();
	}
}

bool Protocol::is_frame_over () const
{
	if (buffer.get_pointer () < 2 || buffer [buffer.get_pointer () - 1] != end_frame)
		return false;

	bool c = true;

	for (auto it = buffer.rbegin () - 1; it != buffer.rend (); ++it)
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
		frame = DataFrames::MessageFrame::parse_frame (buffer);
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
	DataBuffer new_buffer;
	bool prev_guard = false;

	auto type = static_cast<FrameType> (buffer [0]);

	for (size_type i = 1; i < buffer.get_pointer () - 1; ++i)
	{
		if (prev_guard)
		{
			if (buffer [i] == guard || buffer [i] == end_frame)
			{
				new_buffer.append (guard);
				prev_guard = false;
				continue;
			}
			else
				throw std::runtime_error ("syntax error");
		}
		if (buffer [i] == guard)
			prev_guard = true;
		else
			new_buffer.append (buffer [i]);
	}

	if (prev_guard)
		throw std::runtime_error ("syntax error");

	buffer = new_buffer;

	return type;
}

void Protocol::postserialize (DataBuffer& buffer)
{
	DataBuffer new_buffer;

	for (size_type i = 0; i < buffer.get_pointer (); i++)
	{
		if (buffer [i] == guard || buffer [i] == end_frame)
			new_buffer.append (guard);
		new_buffer.append (buffer [i]);
	}

	new_buffer.append (end_frame);
	buffer = new_buffer;
}
