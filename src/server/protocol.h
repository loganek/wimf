/*
 * protocol.h
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "data_types.h"

#include <array>

namespace Wimf {

class Protocol
{
	data_array data;
	data_array::size_type data_pointer = 0;

	bool is_frame_over () const;
	void parse_frame ();
	FrameType preprocess_frame ();

public:
	virtual ~Protocol () {}

	void append_byte (const data_type& byte);
};

}

#endif /* PROTOCOL_H_ */
