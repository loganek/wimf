/*
 * data_types.h
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#ifndef DATA_TYPES_H_
#define DATA_TYPES_H_

#include "databuffer.h"

#include <array>
#include <cstdint>

namespace Wimf {

enum class FrameType
{
	MESSAGE = 0,
	HELLO = 1
};

typedef DataBuffer::data_type data_type;
typedef DataBuffer::data_array::size_type size_type;

typedef std::int64_t user_id;

static constexpr DataBuffer::data_type guard = 220;
static constexpr DataBuffer::data_type end_frame = 0xFF ^ guard; // 0b00100011 = 0x23

}

#endif /* DATA_TYPES_H_ */
