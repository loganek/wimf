/*
 * protocol.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#include "protocol.h"
#include "logger.h"

#include <cstdint>

using namespace Wimf;

void Protocol::append_byte(const data_type& byte)
{
	data [data_pointer++] = byte;

	if (is_frame_over ())
		parse_frame ();

	if (data_pointer == data.size ())
	{
		Logger::log ("client data overflow");
		data_pointer = 0;
	}
}

bool Protocol::is_frame_over () const
{
	if (data_pointer < 2 || data.back () != end_frame)
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
	switch (data.front ())
	{

	}
}
