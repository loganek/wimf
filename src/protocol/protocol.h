/*
 * protocol.h
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "data_types.h"
#include "data_frames/idataframe.h"
#include "databuffer.h"

#include <memory>
#include <functional>
#include <array>

namespace Wimf {

class Protocol
{
	typedef std::function<void(std::shared_ptr<DataFrames::IDataFrame> frame)> frame_callback;

	DataBuffer buffer;
	frame_callback callback;

	bool is_frame_over () const;
	void parse_frame ();
	FrameType preprocess_frame ();

public:
	Protocol (frame_callback callback);
	virtual ~Protocol () {}

	void append_byte (const data_type& byte);

	static void postserialize (DataBuffer& buffer);
};

}

#endif /* PROTOCOL_H_ */
