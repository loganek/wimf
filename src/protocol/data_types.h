/*
 * data_types.h
 *
 *  Created on: Nov 3, 2014
 *      Author: loganek
 */

#ifndef DATA_TYPES_H_
#define DATA_TYPES_H_

#include <cstdint>

namespace Wimf {

enum class FrameType
{
	MESSAGE = 0,
	HELLO = 1,
	LOCATION = 2
};


typedef std::int32_t user_id;
typedef std::int32_t location_serialized_type;

static constexpr double location_divisor = 100000.0;

}

#endif /* DATA_TYPES_H_ */
