/*
 * helloframe.cpp
 *
 *  Created on: Aug 23, 2015
 *      Author: loganek
 */

#include "helloframe.h"
#include "utils.h"

using namespace Wimf::DataFrames;

HelloFrame::HelloFrame (user_id id)
: id (id)
{}

std::shared_ptr<IDataFrame> HelloFrame::parse_frame (const Wimf::DataBuffer& buffer)
{
	constexpr size_type size = sizeof (user_id);

	if (buffer.get_pointer () < size)
		return 0;

	user_id id = convert_to<user_id> (buffer, 0);

	return std::make_shared<HelloFrame> (id);
}


Wimf::DataBuffer HelloFrame::serialize ()
{
	Wimf::DataBuffer ret_data;
	ret_data.append (static_cast<data_type> (FrameType::HELLO));

	for (size_t i = 0; i < sizeof (id); i++)
		ret_data.append (((data_type*)&id) [i]);

	return ret_data;
}
