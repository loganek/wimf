/*
 * locationframe.h
 *
 *  Created on: Aug 24, 2015
 *      Author: loganek
 */

#ifndef SRC_PROTOCOL_DATA_FRAMES_LOCATIONFRAME_H_
#define SRC_PROTOCOL_DATA_FRAMES_LOCATIONFRAME_H_

#include "idataframe.h"

namespace Wimf {
namespace DataFrames {

class LocationFrame : public IDataFrame
{
	user_id id;
	double latitude;
	double longitude;

public:
	virtual ~LocationFrame () {}
	LocationFrame (user_id id, double latitude, double longitude);

	static std::shared_ptr<IDataFrame> parse_frame (const DataBuffer& buffer);

	user_id get_id () const { return id; }
	double get_latitude () const { return latitude; }
	double get_longitude () const { return longitude; }

	FrameType get_frame_type () override { return FrameType::LOCATION; }

	DataBuffer serialize () override;
};

}
}

#endif /* SRC_PROTOCOL_DATA_FRAMES_LOCATIONFRAME_H_ */
