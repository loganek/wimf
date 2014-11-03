/*
 * data_types.h
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#ifndef DATA_TYPES_H_
#define DATA_TYPES_H_

#include <array>
#include <cstdint>

namespace Wimf {

enum class FrameType
{
	MESSAGE = 0,
};

typedef std::array<unsigned char, 128> data_array;
typedef data_array::value_type data_type;

typedef std::int64_t user_id;

static constexpr data_type guard = 220;
static constexpr data_type end_frame = 0xFF ^ guard; // 0b00100011 = 0x23

}

#endif /* DATA_TYPES_H_ */
