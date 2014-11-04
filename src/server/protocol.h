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

#include <memory>
#include <functional>
#include <array>

namespace Wimf {

class Protocol
{
	typedef std::function<void(std::shared_ptr<DataFrames::IDataFrame> frame)> frame_callback;

	data_array data;
	data_array::size_type data_pointer = 0;

	frame_callback callback;

	bool is_frame_over () const;
	void parse_frame ();
	FrameType preprocess_frame ();
	static data_array::size_type postserialize (data_array& frame);

public:
	Protocol (frame_callback callback);
	virtual ~Protocol () {}

	void append_byte (const data_type& byte);
};

}

#endif /* PROTOCOL_H_ */
