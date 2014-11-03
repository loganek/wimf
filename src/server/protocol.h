/*
 * protocol.h
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <array>

namespace Wimf {

class Protocol
{
	typedef std::array<unsigned char, 128> data_array;
	typedef data_array::value_type data_type;

	static constexpr data_type guard = 220;
	static constexpr data_type end_frame = 0xFF ^ guard;

	data_array data;
	data_array::size_type data_pointer = 0;

	bool is_frame_over () const;
	void parse_frame ();

public:
	virtual ~Protocol () {}

	void append_byte (const data_type& byte);
};

}

#endif /* PROTOCOL_H_ */
