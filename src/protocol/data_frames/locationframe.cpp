/*
 * locationframe.cpp
 *
 *  Created on: Aug 24, 2015
 *      Author: loganek
 */

#include "locationframe.h"
#include "utils.h"

using namespace Wimf::DataFrames;

LocationFrame::LocationFrame (user_id id, double latitude, double longitude)
: id (id), latitude (latitude), longitude (longitude)
{}

std::shared_ptr<IDataFrame> LocationFrame::parse_frame (const DataBuffer& buffer)
{
	constexpr size_type size = sizeof (user_id) + sizeof (location_serialized_type) * 2;

	if (buffer.get_pointer () < size)
		return 0;

	user_id id = convert_to<user_id> (buffer, 0);
	double latitude = convert_to <location_serialized_type> (buffer, sizeof (id)) / location_divisor;
	double longitude = convert_to <location_serialized_type> (buffer, sizeof (latitude) + sizeof (id)) / location_divisor;

	return std::make_shared<LocationFrame> (id, latitude, longitude);
}


Wimf::DataBuffer LocationFrame::serialize ()
{
	Wimf::DataBuffer ret_data;
	ret_data.append (static_cast<data_type> (FrameType::LOCATION));

	for (size_t i = 0; i < sizeof (id); i++)
		ret_data.append (((data_type*)&id) [i]);

	location_serialized_type lat = latitude * location_divisor,
			lon = longitude * location_divisor;

	for (size_t i = 0; i < sizeof (lat); i++)
		ret_data.append (((data_type*)&lat) [i]);
	for (size_t i = 0; i < sizeof (lon); i++)
		ret_data.append (((data_type*)&lon) [i]);

	return ret_data;
}
