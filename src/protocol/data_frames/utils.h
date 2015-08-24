/*
 * utils.h
 *
 *  Created on: Aug 23, 2015
 *      Author: loganek
 */

#ifndef SRC_SERVER_DATA_FRAMES_UTILS_H_
#define SRC_SERVER_DATA_FRAMES_UTILS_H_

#include "../data_types.h"

#include <cassert>

namespace Wimf {

template<typename T>
T convert_to(const DataBuffer& buffer, size_type start)
{
	assert (buffer.get_size () >= start + sizeof (T));

	return *((T*)buffer.get_data());
}

}

#endif /* SRC_SERVER_DATA_FRAMES_UTILS_H_ */
